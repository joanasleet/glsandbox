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

ShaderCache* shaderCache;
UniformCache* uniformsCache;

void a() {
    printf("func a();\n");
}

void b() {
    printf("func b();\n");
}

void c() {
    printf("func c();\n");
}

void (* funcs[])() = {a, b, c};

int main(int argc, char** argv) {

    config();
   
    for (int i = 0; i < 3; ++i) {
        (* funcs[i])();
    }

    /* render loop */
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        /*
        fps();
        printWatchLog();
        update(cam);
         * */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
        updateMVP(floorShader, mvpLoc[0]);
        glUseProgram(floorShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(planeTex->target, planeTex->id);
        glBindVertexArray(planeVao->id);
        glDrawArrays(GL_QUADS, 0, 4);

        updateMVP(cubeShader, mvpLoc[1]);
        glUseProgram(cubeShader);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(cubeTex->target, cubeTex->id);
        glBindVertexArray(cubeVao->id);
        glDrawArrays(GL_QUADS, 0, 24);

        updateMVP(lcubeShader, mvpLoc[2]);
        glUseProgram(lcubeShader);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(lcubeTex->target, lcubeTex->id);
        glBindVertexArray(lcubeVao->id);
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