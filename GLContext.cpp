#include "GLContext.h"

GLContext::GLContext(int resX, int resY, const char* title) {
    setRes(resX, resY);

    if (!glfwInit()) {
        printError("Failed to start GLFW.");
        exit(EXIT_FAILURE);
    }

    printf("GLFW initialized.\n");

    main = glfwCreateWindow(resX, resY, title, NULL, NULL);
    if (!main) {
        printError("Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(main);
    printf("Main window created.\n");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printError("Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    printf("GLEW initialized.\n");
}

GLContext::~GLContext() {
    glfwTerminate();
}

void GLContext::setRes(int x, int y) {
    resX = x;
    resY = y;
}
