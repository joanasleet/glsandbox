#include "GLContext.h"

GLContext::GLContext(int resX, int resY, const char* title) : resX(resX), resY(resY) {

    if (!glfwInit()) {
        ERR("Failed to start GLFW.");
        exit(EXIT_FAILURE);
    }

    INFO("GLFW initialized.");

    main = glfwCreateWindow(resX, resY, title, NULL, NULL);
    if (!main) {
        ERR("Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(main);
    INFO("Main window created.");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        ERR("Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    INFO("GLEW initialized.");
}

GLContext::~GLContext() {
    glfwTerminate();
}

void GLContext::config() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glfwSetInputMode(main, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void GLContext::printSpec() {
    
}

