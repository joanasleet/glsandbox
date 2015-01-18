#include "Engine.h"
#include "Debugger.h"
#include "Timestep.h"
#include "ShaderUtil.h"
#include "Deallocator.h"
#include "InputManager.h"
#include "LookupManager.h"

#include <math.h>
#include <string.h>

static float renderAlpha = 0.0f;

Engine *init() {

    Engine *renderer = (Engine *) malloc(sizeof (Engine));

    renderer->context = newContext();
    renderer->mainCam = newCamera(0.0f, 0.0f, 0.0f);

    renderer->shaderCache = newCache();
    renderer->uniformCache = newCache();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    GLFWwindow *window = renderer->context->win;

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorEnterCallback(window, cursorEnterCB);
    glfwSetScrollCallback(window, scrollCB);
    glfwSetKeyCallback(window, keyCB);
    glfwSetWindowSizeCallback(window, resizeCB);

    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return renderer;
}

void preload(Object *obj, Engine *renderer) {

    Shader *shader = obj->shader;
    GLint prog = shader->program;
    const char *shaderStage;

    /* cache shaders */
    for (int i = 0; i < shader->stageCount; ++i) {
        shaderStage = shader->stages[i];

        if (strlen(shaderStage) > 0) {
            addShader(shaderStage, ShaderType[i], prog, renderer->shaderCache);
        }
    }

    /* cache uniform locations */
    GLint loc;
    const char *str;
    const char *key;

    for (int i = 0; i < shader->uniformCount; ++i) {
        str = shader->uniforms[i];

        loc = glGetUniformLocation(prog, str);
        if (loc < 0)
            log_warn("[PRELOAD] missing uniform '%s' in shader prog %i (loc: %i)", str, prog, loc);

        key = getKey(str, prog);
        cache(renderer->uniformCache, key, loc);
    }
}

void render(Object *obj, Engine *renderer) {

    /*
     * interpolate camera */
    Camera *cam = renderer->mainCam;

    /* save old state */
    State *oldState = cam->state;

    /* setup interpolated state */
    State interpolatedState;

    /* setup position */
    float position[3];
    setPosition(position, oldState, renderAlpha);
    interpolatedState.position = position;

    /* setup angles */
    float angles[3];
    setAngles(angles, oldState, renderAlpha);
    interpolatedState.angles = angles;

    /* set interpolated state */
    cam->state = &interpolatedState;

    // special sun stuff
    glUseProgram(1);
    double t = glfwGetTime();
    glUniform1f(glGetUniformLocation(1, "time"), t);
    //double x = 1.0 - (0.5 * (cosf(0.1 * t) + 1.0));
    //watch("x tex coord: %.10f\n", x);

    /*
     * update uniforms */
    GLint loc;
    char *tempstr;
    Shader *shader = obj->shader;
    GLint program = shader->program;
    glUseProgram(program);

    for (int i = 0; i < shader->uniformCount; ++i) {
        tempstr = (char *) getKey(shader->uniforms[i], program);
        loc = get(renderer->uniformCache, tempstr);
        free(tempstr);

        // TODO: interpolate object state

        (*shader->setters[i])(loc, cam, obj->state);
    }

    /*
     * reset old state */
    cam->state = oldState;

    /*
     * bind materials */
    Material *mat = obj->mats;

    Texture *tex;
    for (uint32 i = 0; i < mat->texCount; ++i) {

        if (i > maxTexSlots - 1) {
            log_warn("Texture bind limit (%d) reached.", maxTexSlots);
            break;
        }

        tex = mat->textures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(tex->target, tex->id);
    }

    /*
     * draw object */
    Mesh *mesh = obj->mesh;
    glBindVertexArray(mesh->vaoId);
    mesh->draw(mesh->mode, &mesh->first, mesh->count);
}

void exitIfNoObjects(Engine *renderer) {
    exit_guard(renderer->objects[0]);
}

void preloadObjects(Engine *renderer) {
    log_info("%s", "- - - - - Preloading objects - - - - -");
    for (unsigned int i = 0; i < renderer->objectCount; ++i) {
        preload(renderer->objects[i], renderer);
    }
}

void renderObjects(Engine *renderer) {

    for (unsigned int i = 0; i < renderer->objectCount; ++i) {
        render(renderer->objects[i], renderer);
    }
}

void enterLoop(Engine *renderer) {

    exit_guard(renderer);

    exitIfNoObjects(renderer);
    preloadObjects(renderer);

    Camera *cam = renderer->mainCam;
    GLFWwindow *window = renderer->context->win;

    double dt, lag = 0.0;

    startTimer();

    log_info("%s", "- - - - - - - Rendering - - - - - - -");
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        dt = elapsedTime();
        lag += dt;

        fps(dt);

        while ( lag >= 0.016) {

            updateCam(cam);
            lag -= 0.016;
        }

        renderAlpha = (float) (lag / 0.016);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderObjects(renderer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void terminate(Engine *renderer) {

    freeCache(renderer->shaderCache);
    freeCache(renderer->uniformCache);

    freeObjects(renderer);

    free(renderer->context);

    freeCamera(renderer->mainCam);

    deallocStores();

    free(renderer);

    glfwTerminate();
}

void freeObjects(Engine *renderer) {

    Object *freeMe;
    for (uint32 i = 0; i < renderer->objectCount; ++i) {
        freeMe = renderer->objects[i];
        freeObject(freeMe);
        renderer->objects[i] = NULL;
    }
    free(renderer->objects);
    renderer->objects = NULL;
}