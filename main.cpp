#include "MeshUtil.h"
#include "Camera.h"
#include "Context.h"
#include "Logger.h"
#include "ShaderCache.h"

#include <iostream>

using namespace std;

/* opengl context */
Context* context = newContext();

Camera* cam = createCamera(0.0f, -4980.0f, 1000.0f);

ShaderCache* shaderCache = newCache();
UniformCache* uniformCache = newCache();

Mesh* meshes[MAX_MESHES];
int nextMeshSlot = 0;

int main(int argc, char** argv) {

    config();

    Mesh* plane = newMesh();
    plane->vaoId = planeVAO(20000.0f, 50.0f, 0.0f, -5000.0f, 0.0f);
    plane->tex = newTexture("textures/concrete.jpg");
    const char* uniforms[] = {"MV", "P"};
    plane->uniforms = uniforms;
    void (*funcs[])(GLint) = {MV, P};
    plane->setUniformFunc = funcs;
    plane->uniLen = 2;
    const char* shaders[] = {"tex_light.vert", "tex_light.frag"};
    plane->shaders = shaders;
    plane->shadersLen = 2;
    plane->drawFunc = drawArrays;
    plane->mode = GL_QUADS;
    plane->first = 0;
    plane->count = 4;
    ADD_MESH(plane);

    Mesh* cube = newMesh();
    cube->vaoId = cubeVAO(500.0f, 20.0f);
    cube->tex = newTexture("textures/brick.png");
    const char* uniforms2[] = {"MV", "P"};
    cube->uniforms = uniforms2;
    void (*funcs2[])(GLint) = {MV, P};
    cube->setUniformFunc = funcs2;
    cube->uniLen = 2;
    const char* shaders2[] = {"tex_light.vert", "tex_light.frag"};
    cube->shaders = shaders2;
    cube->shadersLen = 2;
    cube->drawFunc = drawArrays;
    cube->mode = GL_QUADS;
    cube->first = 0;
    cube->count = 24;
    //ADD_MESH(cube);

    Mesh* cubemap = newMesh();
    cubemap->vaoId = cubeMapVAO(100000.0f);
    const char* faces[] = {
        "textures/SBRIGHT.png",
        "textures/SBLEFT.png",
        "textures/SBTOP.png",
        "textures/SBBOTTOM.png",
        "textures/SBFRONT.png",
        "textures/SBBACK.png"
    };
    cubemap->tex = cubeMap(faces);
    const char* uniforms3[] = {"MVP"};
    cubemap->uniforms = uniforms3;
    void (*funcs3[])(GLint) = {MVP};
    cubemap->setUniformFunc = funcs3;
    cubemap->uniLen = 1;
    const char* shaders3[] = {"cubemap.vert", "cubemap.frag"};
    cubemap->shaders = shaders3;
    cubemap->shadersLen = 2;
    cubemap->drawFunc = drawArrays;
    cubemap->mode = GL_QUADS;
    cubemap->first = 0;
    cubemap->count = 24;
    ADD_MESH(cubemap);

    /* DONT TOUCH ANYTHING FROM HERE ON*/
    if (!meshes[0]) {
        INFO("No meshes registered. Aborting.");
        exit(0);
    }

    INFO("Preloading meshes...");
    for (int i = 0; i < nextMeshSlot; ++i) {
        preload(meshes[i]);

    }

    CATCH_EXCEPTION;
    INFO("–––––––––– Rendering ––––––––––");
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        fps();
        printWatchLog();
        update(cam);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < nextMeshSlot; ++i) {
            render(meshes[i]);
        }


        /*
        glUseProgram(plane->shaderProgram);
        GLint loc = glGetUniformLocation(plane->shaderProgram, "MVP");
        INFO("%i", loc);
        glm::mat4 P = *(cam->perspective);
        glm::mat4 MV = *(cam->modelview);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(P * MV));

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