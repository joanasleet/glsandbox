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
UniformCache* uniformsCache = newCache();

// djb2

unsigned long hashh(unsigned char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int main(int argc, char** argv) {

    config();

    char s1[] = "asdad";
    const char* s2 = (const char*) s1;
    
    

    cache(shaderCache, "Test", 1337);
    cache(shaderCache, "Test2", 1227);

    unsigned char str[] = "1_Light";

    printf("%lu\n", hashh(str) % 10);

    exit(0);

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