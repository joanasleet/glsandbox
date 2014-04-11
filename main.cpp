#include <cstdlib>
#include <stdio.h>

#include "GLContext.h"

using namespace std;

int main(int argc, char** argv) {

    GLContext context;

    context.loadShader("vertex", GL_VERTEX_SHADER);
    context.loadShader("fragment", GL_FRAGMENT_SHADER);

    while (!glfwWindowShouldClose(context.main)) {

        context.applyShaders();

        /* --- rendering --- */

        /* ---------------- */

        glfwSwapBuffers(context.main);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

