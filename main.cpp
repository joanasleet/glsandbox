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

    // try with non tesselated mesh
    Mesh* terrain = newMesh();
    {
        const char* uniforms[] = {"P", "MV"};
        void (*funcs[])(GLint) = {P, MV};
        const char* shaders[] = {"base_tess.vert", "tex.frag", "control.tess", "eval.tess"};

        terrain->vaoId = terrainVAO(10000.0f, 10000.0f);
        terrain->tex = newTexture("textures/white.png");
        terrain->uniforms = uniforms;
        terrain->setUniformFunc = funcs;
        terrain->uniLen = 2;
        terrain->shaders = shaders;
        terrain->shadersLen = 4;
        terrain->drawFunc = drawArrays;
        terrain->mode = GL_PATCHES;
        terrain->first = 0;
        terrain->count = 4;
        glPatchParameteri(GL_PATCH_VERTICES, 4);
    }
    //preload(terrain);

    Mesh* plane = newMesh();
    {
        const char* uniforms[] = {"P", "MV"};
        void (*funcs[])(GLint) = {P, MV};
        const char* shaders[] = {"base.vert", "tex.frag"};

        plane->vaoId = planeVAO(10000.0f, 100.0f);
        plane->tex = newTexture("textures/concrete.jpg");
        plane->uniforms = uniforms;
        plane->setUniformFunc = funcs;
        plane->uniLen = 2;
        plane->shaders = shaders;
        plane->shadersLen = 2;
        plane->drawFunc = drawArrays;
        plane->mode = GL_QUADS;
        plane->first = 0;
        plane->count = 4;
    }
    preload(plane);

    Mesh* skybox = newMesh();
    {
        const char* uniforms[] = {"MVP"};
        void (*funcs[])(GLint) = {MVPnoTrans};
        const char* shaders[] = {"cubemap.vert", "cubemap.frag"};

        const char* cubefaces[] = {
            "textures/xn.bmp",
            "textures/xp.bmp",
            "textures/yp.bmp",
            "textures/yn.bmp",
            "textures/zp.bmp",
            "textures/zn.bmp"
        };

        skybox->vaoId = cubeMapVAO(2.0f);
        skybox->tex = cubeMap(cubefaces, false, false);
        skybox->uniforms = uniforms;
        skybox->setUniformFunc = funcs;
        skybox->uniLen = 1;
        skybox->shaders = shaders;
        skybox->shadersLen = 2;
        skybox->drawFunc = drawArrays;
        skybox->mode = GL_QUADS;
        skybox->first = 0;
        skybox->count = 24;
    }
    preload(skybox);


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    //exitIfNoMeshes();
    //preloadMeshes();

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glDepthRange(0.1f, 1.0f);

    INFO("–––––––––– Rendering ––––––––––");
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {



        fps();
        //printWatchLog();
        update(cam);

        glClear(GL_COLOR_BUFFER_BIT);

        /*
        glDepthMask(GL_FALSE);
        render(skybox);
        glDepthMask(GL_TRUE);
         * */


        //glDepthMask(GL_FALSE);

        glUseProgram(skybox->shaderProgram);
        MVPnoTrans(glGetUniformLocation(skybox->shaderProgram, "MVP"));
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->tex->id);
        glBindVertexArray(skybox->vaoId);
        glDrawArrays(GL_QUADS, 0, 24);

        //glDepthMask(GL_TRUE);

        glClear(GL_DEPTH_BUFFER_BIT);

        glUseProgram(plane->shaderProgram);
        P(glGetUniformLocation(plane->shaderProgram, "P"));
        MV(glGetUniformLocation(plane->shaderProgram, "MV"));
        glBindTexture(GL_TEXTURE_2D, plane->tex->id);
        glBindVertexArray(plane->vaoId);
        glDrawArrays(GL_QUADS, 0, 4);

        // das laeuft wohl richtig
        //render(plane);
        //render(skybox);

        glFlush();
        glfwSwapBuffers(context->win);
        glfwPollEvents();
    }

    free(context);
    free(cam);

    glfwTerminate();

    return EXIT_SUCCESS;
}

void exitIfNoMeshes() {
    if (!meshes[0]) {
        INFO("No meshes registered. Aborting.");
        exit(0);
    }
}

void preloadMeshes() {
    INFO("–––––– Preloading meshes ––––––");
    for (int i = 0; i < nextMeshSlot; ++i) {
        preload(meshes[i]);
    }
}

void renderMeshes() {
    for (int i = 0; i < nextMeshSlot; ++i) {
        render(meshes[i]);
    }
}

void add(Mesh* mesh) {
    if (nextMeshSlot > MAX_MESHES - 1) {
        ERR("Max meshes reached.");
        return;
    }
    meshes[nextMeshSlot++] = mesh;
}