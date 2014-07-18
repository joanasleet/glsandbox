#include "MeshUtil.h"
#include "Camera.h"
#include "Context.h"
#include "Logger.h"
#include "ShaderCache.h"

#include <iostream>

using namespace std;

/* opengl context */
Context* context = newContext();

Camera* cam = createCamera(0.0f, 0.0f, 150.0f);

ShaderCache* shaderCache = newCache();
UniformCache* uniformCache = newCache();

Mesh* meshes[MAX_MESHES];
int nextMeshSlot = 0;

glm::mat4 perspective;
glm::mat4 modelview;

int main(int argc, char** argv) {

    config();

    Mesh* plane = newMesh();
    plane->vaoId = cubeVAO(50.0f, 20.0f);
    plane->tex = newTexture("textures/tiles.png");
    const char* uniforms[] = {"MVP"};
    plane->uniforms = uniforms;
    void (*funcs[])(GLint) = {MVP};
    plane->setUniformFunc = funcs;
    plane->uniLen = 1;
    const char* shaders[] = {"base.vert", "tex.frag"};
    plane->shaders = shaders;
    plane->shadersLen = 2;
    plane->drawFunc = drawArrays;
    plane->mode = GL_QUADS;
    plane->first = 0;
    plane->count = 24;
    ADD_MESH(plane);


    if (!meshes[0]) {
        INFO("No meshes registered. Aborting.");
        exit(0);
    }


    INFO("Preloading meshes...");
    for (int i = 0; i < nextMeshSlot; ++i) {
        preload(meshes[i]);

    }

   
    INFO("–––––––––– Rendering ––––––––––");
    CATCH_EXCEPTION;
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        fps();
        printWatchLog();
        update(cam);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //for (int i = 0; i < nextMeshSlot; ++i) {
        //    render(meshes[i]);
        //}

        /*
        glUseProgram(plane->shaderProgram);
        GLint loc = glGetUniformLocation(plane->shaderProgram, "MVP");
        glm::mat4 MVP = perspective*modelview;
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));

        glUseProgram(plane->shaderProgram);
        glBindVertexArray(plane->vaoId);
        glBindTexture(GL_TEXTURE_2D, plane->tex->id);
        glDrawArrays(GL_QUADS, 0, 24);
        */

        glFlush();
        glfwSwapBuffers(context->win);
        glfwPollEvents();
    }

    free(context);
    free(cam);

    glfwTerminate();

    return EXIT_SUCCESS;
}