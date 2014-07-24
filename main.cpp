#include "MeshUtil.h"
#include "Camera.h"
#include "Context.h"
#include "Logger.h"
#include "ShaderCache.h"

#include <iostream>

using namespace std;

/* opengl context */
Context* context = newContext();

Camera* cam = createCamera();

ShaderCache* shaderCache = newCache();
UniformCache* uniformCache = newCache();

Mesh* meshes[MAX_MESHES];
int nextMeshSlot = 0;

int main(int argc, char** argv) {

    config();

    Mesh* terrain = newMesh();
    terrain->vaoId = terrainVAO(1000.0f, 1000.0f);
    terrain->tex = newTexture("textures/brick.png");
    const char* uniforms[] = {"MV", "P"};
    terrain->uniforms = uniforms;
    void (*funcs[])(GLint) = {MV, P};
    terrain->setUniformFunc = funcs;
    terrain->uniLen = 2;
    const char* shaders[] = {"base.vert", "tex.frag", "control.tess", "eval.tess"};
    terrain->shaders = shaders;
    terrain->shadersLen = 4;
    terrain->drawFunc = drawArrays;
    terrain->mode = GL_PATCHES;
    terrain->first = 0;
    terrain->count = 6;
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    

    ADD_MESH(terrain);


    /* DONT TOUCH ANYTHING FROM HERE ON */
    if (!meshes[0]) {
        INFO("No meshes registered. Aborting.");
        exit(0);
    }

    INFO("–––––– Preloading meshes ––––––");
    for (int i = 0; i < nextMeshSlot; ++i) {
        preload(meshes[i]);

    }

    INFO("–––––––––– Rendering ––––––––––");
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        fps();
        printWatchLog();

        update(cam);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < nextMeshSlot; ++i) {
            render(meshes[i]);
        }

        glFlush();
        glfwSwapBuffers(context->win);
        glfwPollEvents();
    }

    free(context);
    free(cam);

    glfwTerminate();

    return EXIT_SUCCESS;
}