#include "GLContext.h"

GLContext::GLContext(int resX, int resY, const char* title) {
    setRes(resX, resY);

    if (!glfwInit()) {
        fprintf(stderr, "ERROR starting GLFW.\n");
        exit(EXIT_FAILURE);
    }

    printf("GLFW initialized.\n");

    main = glfwCreateWindow(resX, resY, title, NULL, NULL);
    if (!main) {
        fprintf(stderr, "ERROR creating main window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(main);
    printf("Window created.\n");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("ERROR starting GLEW.\n");
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
