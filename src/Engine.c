#include "Engine.h"
#include "Debugger.h"
#include "string.h"
#include "ShaderUtil.h"
#include "Deallocator.h"

//#include <time.h>
//#include <sys/time.h>

#include <unistd.h>

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

    renderer->context = newContext();
    renderer->mainCam = newCam();

    renderer->shaderCache = newCache();
    renderer->uniformCache = newCache();

    glEnable(GL_DEPTH_TEST);

#if (CULL_FACES)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
#endif

    GLFWwindow* window = renderer->context->win;

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
     * normal and specular are optional */
    Texture* spec = mat->specularMap;
    if (spec) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(spec->target, spec->id);
    }

    Texture* normals = mat->normalMap;
    if (normals) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(normals->target, normals->id);
    }

    glBindVertexArray(mesh->vaoId);
    (*mesh->drawFunc)(mesh->mode, &mesh->first, mesh->count);
}

void exitIfNoMeshes(Engine* renderer) {
    exit_guard(renderer->meshes[0]);
}

void preloadMeshes(Engine* renderer) {
    info("%s", "–––––– Preloading meshes ––––––");
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

    exit_guard(renderer);

    exitIfNoMeshes(renderer);
    preloadMeshes(renderer);

    // move to timer class
    //double startTime, endTime;
    useconds_t usec = 16000;

    Camera* cam = renderer->mainCam;
    GLFWwindow* window = renderer->context->win;

    info("%s", "–––––––––– Rendering ––––––––––");
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        //printCache(renderer->uniformCache, stdout);
        //printStores();

        //startTime = glfwGetTime();
        //usec = 16000;

        fps();
        update(cam);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderMeshes(renderer);
        

        glfwSwapBuffers(window);
        glfwPollEvents();

        // encapsulate into timer class
        //endTime = glfwGetTime();
        //usec += (long) ((startTime - endTime)*1000000.0);
        usleep(usec);
    }
}

void terminate(Engine* renderer) {

    freeCache(renderer->shaderCache);
    freeCache(renderer->uniformCache);
    
    freeMeshes(renderer);

    free(renderer->context);
    
    free(renderer->mainCam->perspective);
    free(renderer->mainCam->translation);
    free(renderer->mainCam->orientation);

    free(renderer->mainCam->angles);
    free(renderer->mainCam->forward);
    free(renderer->mainCam->speed);
    free(renderer->mainCam->position);
            
    free(renderer->mainCam);
    
    deallocStores();

    free(renderer);
    
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