#include "Context.h"
#include "Camera.h"
#include "Logger.h"
#include "Engine.h"

extern Engine* renderer;

Context* newContext(unsigned int xRes, unsigned int yRes, const char* title) {
    INFO("–––––––––––––––– Log Start ––––––––––––––––––");
    Context* context = (Context*) malloc(sizeof (Context));
    context->xRes = xRes;
    context->yRes = yRes;

    glfwSetErrorCallback(contextErrorCB);

    if (!glfwInit()) {
        ERR("Failed to start GLFW.");
        exit(EXIT_FAILURE);
    }

    INFO("GLFW initialized.");

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    //    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    context->win = glfwCreateWindow(context->xRes, context->yRes, title, NULL, NULL);
    if (!context->win) {
        ERR("Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(context->win);
    INFO("Main window created.");
    glfwWindowHint(GLFW_SAMPLES, 4);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        ERR("Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    INFO("GLEW initialized.");

    if (GLEW_KHR_debug) {
        int param = -1;
        const void* p = &param;
  //      glDebugMessageCallback(debugCB, NULL);
        //glDebugMessageCallback(debugCB);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }



    return context;
}

void contextErrorCB(int code, const char* msg) {
    ERR("In context - %s (Code %i)", msg, code);
}

void resizeCB(GLFWwindow* win, int w, int h) {
    renderer->glContext->xRes = w;
    renderer->glContext->yRes = h;
    glViewport(0, 0, w, h);
}

void fps() {
    static double prevTime = glfwGetTime();
    double currTime = glfwGetTime();
    double diffTime = currTime - prevTime;
    static int frames = 0;

    if (diffTime > 0.5) {
        prevTime = currTime;
        char title[20];
        sprintf(title, "OpenGL @ %.2f", (double) frames / diffTime);
        glfwSetWindowTitle(renderer->glContext->win, title);
        frames = 0;
    }
    ++frames;
}

double elapsedTime() {

    static double startTime = glfwGetTime();
    return ( glfwGetTime() - startTime);
}

void resetTimer() {
    glfwSetTime(0.0);
}