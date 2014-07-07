#include "Context.h"

Context* createContext(unsigned int xRes, unsigned int yRes, const char* title) {
    Context* context = (Context*) malloc(sizeof (Context));
    context->xRes = xRes;
    context->yRes = yRes;

    if (!glfwInit()) {
        ERR("Failed to start GLFW.");
        exit(EXIT_FAILURE);
    }

    INFO("GLFW initialized.");
        
    context->win = glfwCreateWindow(context->xRes, context->yRes, title, NULL, NULL);
    if (!context->win) {
        ERR("Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(context->win);
    INFO("Main window created.");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        ERR("Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    INFO("GLEW initialized.");

    return context;
}
