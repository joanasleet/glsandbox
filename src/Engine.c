#include "Engine.h"
#include "Debugger.h"
#include "Timestep.h"
#include "ShaderUtil.h"
#include "Deallocator.h"
#include "InputManager.h"
#include "LookupManager.h"

#include <math.h>
#include <string.h>

#define SGN(x) ((x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f))

static float renderAlpha = 0.0f;

Engine *init() {

    Engine *renderer = (Engine *) malloc(sizeof (Engine));

    renderer->context = newContext();
    renderer->mainCam = newCamera(0.0f, 0.0f, 0.0f);

    renderer->shaderCache = newCache();
    renderer->uniformCache = newCache();

    glEnable(GL_DEPTH_TEST);

#if (CULL_FACES)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
#endif

    GLFWwindow *window = renderer->context->win;

    glClearColor(0.2, 0.5, 1.0, 1.0);
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
        key = getKey(str, prog);
        cache(renderer->uniformCache, key, loc);
    }
}

void render(Object *obj, Engine *renderer) {

    // printf("Rendering at %.0f%%.\n", renderAlpha * 100.f);

    Camera *cam = renderer->mainCam;

    // save old state
    State *oldState = cam->state;
    State newState;

    float position[3];
    position[0] = oldState->position[0] + (cam->forward[0] * oldState->velocity[2] + cam->right[0] * oldState->velocity[0] + cam->up[0] * oldState->velocity[1]) * renderAlpha;
    position[1] = oldState->position[1] + (cam->forward[1] * oldState->velocity[2] + cam->right[1] * oldState->velocity[0] + cam->up[1] * oldState->velocity[1]) * renderAlpha;
    position[2] = oldState->position[2] + (cam->forward[2] * oldState->velocity[2] + cam->right[2] * oldState->velocity[0] + cam->up[2] * oldState->velocity[1]) * renderAlpha;

    float angles[3];
    angles[0] = oldState->angles[0] + oldState->angleVelocity[0] * renderAlpha;
    angles[1] = oldState->angles[1] + oldState->angleVelocity[1] * renderAlpha;
    angles[2] = oldState->angles[2] + oldState->angleVelocity[2] * renderAlpha;

    newState.position = position;
    newState.angles = angles;

    // set interpolated state
    cam->state = &newState;

    /* calc orientation from
    new state and set to cam */

    GLint loc;
    Shader *shader = obj->shader;
    GLint program = shader->program;
    glUseProgram(program);

    char *tempstr;

    /*
     * update uniforms */
    for (int i = 0; i < shader->uniformCount; ++i) {
        tempstr = (char *) getKey(shader->uniforms[i], program);
        loc = get(renderer->uniformCache, tempstr);
        free(tempstr);

        (*shader->setters[i])(loc, renderer->mainCam, obj->state);
    }

    // set old state
    cam->state = oldState;

    // State *state = cam->state;

    // update angle velocity
    // static double lastMousePos[2];
    // double currMousePos[2];
    // glfwGetCursorPos(renderer->context->win, currMousePos, currMousePos + 1);
    // // PULSE( info("(%.1f, %.1f)", currMousePos[0], currMousePos[1]), 10);

    // if (    (lastMousePos[0] - currMousePos[0]) < 0.0000001 &&
    //         (lastMousePos[1] - currMousePos[1]) < 0.0000001 ) {

    //     /* if last updates state equals this one
    //        then mouse was not moved */
    //     // state->angleVelocity[0] -= copysignf(0.001f, state->angleVelocity[0]);
    //     // state->angleVelocity[1] -= copysignf(0.001f, state->angleVelocity[1]);
    //     // state->angleVelocity[2] -= copysignf(0.001f, state->angleVelocity[2]);

    //     // state->angleVelocity[0] = fmax(state->angleVelocity[0], 0.0f);
    //     // state->angleVelocity[1] = fmax(state->angleVelocity[1], 0.0f);
    //     // state->angleVelocity[2] = fmax(state->angleVelocity[2], 0.0f);

    //     state->angleVelocity[0] = 0.0f;
    //     state->angleVelocity[1] = 0.0f;
    //     state->angleVelocity[2] = 0.0f;
    // } else {

    //     // save for next update
    //     lastMousePos[0] = currMousePos[0];
    //     lastMousePos[1] = currMousePos[1];
    // }


    //info("after render (%.1f, %.1f, %.1f)", cam->state->angleVelocity[0], cam->state->angleVelocity[1], cam->state->angleVelocity[2]);

    Material *mat = obj->mats;

    /*
     * at least bind diffuse map */
    Texture *diff = mat->diffuseMap;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(diff->target, diff->id);

    /*
     * normal and specular are optional */
    Texture *spec = mat->specularMap;
    if (spec) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(spec->target, spec->id);
    }

    Texture *normals = mat->normalMap;
    if (normals) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(normals->target, normals->id);
    }

    /*
     * draw object */
    Mesh *mesh = obj->mesh;
    glBindVertexArray(mesh->vaoId);
    (*mesh->draw)(mesh->mode, &mesh->first, mesh->count);
}

void exitIfNoObjects(Engine *renderer) {
    exit_guard(renderer->objects[0]);
}

void preloadObjects(Engine *renderer) {
    info("%s", "~ ~ ~ ~ Preloading objects ~ ~ ~ ~");
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

    info("%s", "# # # # # # #  Rendering  # # # # # # #");
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