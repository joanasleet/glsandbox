#include "Engine.h"
#include "Debugger.h"
#include "string.h"
#include "ShaderUtil.h"
#include "Deallocator.h"

GLenum ShaderType[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER
};

Engine* init() {

    Engine* renderer = (Engine*) malloc(sizeof (Engine));

    renderer->glContext = newContext();
    renderer->mainCam = createCamera();

    renderer->shaderCache = newCache();
    renderer->uniformCache = newCache();

    glEnable(GL_DEPTH_TEST);

#if (CULL_FACES)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
#endif

    GLFWwindow* window = renderer->glContext->win;

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorEnterCallback(window, cursorEnterCB);
    glfwSetScrollCallback(window, scrollCB);
    glfwSetKeyCallback(window, keyCB);
    glfwSetWindowSizeCallback(window, resizeCB);

    //glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexSlots);

    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return renderer;
}

void add(Mesh* mesh, Engine* renderer) {
    // replaced by loadScene()
}

void preload(Mesh* mesh, Engine* renderer) {

    GLint prog = mesh->shaderProgram;
    const char* shader;

    /* cache shaders */
    for (int i = 0; i < mesh->shadersLen; ++i) {
        shader = mesh->shaders[i];

        if (strlen(shader) > 0) {
            addShader(shader, ShaderType[i], prog, renderer->shaderCache);
        }
    }

    /* cache uniform locations */
    GLint loc;
    const char* str;
    const char* key;

    for (int i = 0; i < mesh->uniLen; ++i) {
        str = mesh->uniforms[i];
        loc = glGetUniformLocation(prog, str);
        key = getKey(str, prog);
        cache(renderer->uniformCache, key, loc);
    }
}

void render(Mesh* mesh, Engine* renderer) {

    GLint loc;
    glUseProgram(mesh->shaderProgram);

    char* tempstr;

    for (int i = 0; i < mesh->uniLen; ++i) {
        tempstr = (char*) getKey(mesh->uniforms[i], mesh->shaderProgram);
        loc = get(renderer->uniformCache, tempstr);
        free(tempstr);
        (*mesh->setUniformFunc[i])(loc, renderer->mainCam);
    }

    Material* mat = mesh->mats;

    /*
     * at least bind diffuse map */
    Texture* diff = mat->diffuseMap;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(diff->target, diff->id);

    /*
     * ambient and specular are optional */
    Texture* amb = mat->ambientMap;
    if (amb) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(amb->target, amb->id);
    }

    Texture* spec = mat->specularMap;
    if (spec) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(spec->target, spec->id);
    }

    glBindVertexArray(mesh->vaoId);
    (*mesh->drawFunc)(mesh->mode, &mesh->first, mesh->count);
}

void exitIfNoMeshes(Engine* renderer) {
    exit_guard(renderer->meshes[0]);
}

void preloadMeshes(Engine* renderer) {
    info("–––––– Preloading meshes ––––––");
    for (unsigned int i = 0; i < renderer->meshCount; ++i) {
        preload(renderer->meshes[i], renderer);
    }
}

void renderMeshes(Engine* renderer) {
    for (unsigned int i = 0; i < renderer->meshCount; ++i) {
        render(renderer->meshes[i], renderer);
    }
}

void enterLoop(Engine* renderer) {

    exitIfNoMeshes(renderer);
    preloadMeshes(renderer);

    // move to timer class
    double startTime, endTime;
    struct timespec msSleep, msRemaining;
    msSleep.tv_sec = 0;
    msSleep.tv_nsec = 16000000;

    Camera* cam = renderer->mainCam;
    GLFWwindow* window = renderer->glContext->win;

    info("–––––––––– Rendering ––––––––––");
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        //printCache(renderer->uniformCache, stdout);
        //printStores();

        startTime = glfwGetTime();
        msSleep.tv_nsec = 16000000;

        fps();
        update(cam);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderMeshes(renderer);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // encapsulate into timer class
        endTime = glfwGetTime();
        msSleep.tv_nsec += (long) ((startTime - endTime)*1000000000.0);
        nanosleep(&msSleep, &msRemaining);
    }
}

void terminate(Engine* renderer) {

    freeCache(renderer->shaderCache);
    renderer->shaderCache = NULL;
    freeCache(renderer->uniformCache);
    renderer->uniformCache = NULL;

    freeMeshes(renderer);

    free(renderer->glContext);
    renderer->glContext = NULL;

    free(renderer->mainCam->perspective);
    free(renderer->mainCam->translation);
    free(renderer->mainCam->orientation);

    renderer->mainCam->perspective = NULL;
    renderer->mainCam->translation = NULL;
    renderer->mainCam->orientation = NULL;

    free(renderer->mainCam);
    renderer->mainCam = NULL;

    deallocStores();

    free(renderer);
    renderer = NULL;

    glfwTerminate();
}

void freeMeshes(Engine* renderer) {

    Mesh* freeMe;
    for (uint32 i = 0; i < renderer->meshCount; i++) {
        freeMe = renderer->meshes[i];
        freeMesh(freeMe);
        renderer->meshes[i] = NULL;
    }
    free(renderer->meshes);
    renderer->meshes = NULL;
}