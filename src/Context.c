#include "Context.h"
#include "Camera.h"
#include "Debugger.h"
#include "Engine.h"

#include "LookupManager.h"

extern Engine *renderer;

Context *createContext(uint32 xRes, uint32 yRes, const char *title) {
    info("%s", "–––––––––––––––– Log Start ––––––––––––––––––");
    Context *context = (Context *) malloc(sizeof (Context));
    context->xRes = xRes;
    context->yRes = yRes;

    glfwSetErrorCallback(contextErrorCB);

    exit_guard(glfwInit());

    info("%s", "GLFW initialized.");

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    context->win = glfwCreateWindow(context->xRes, context->yRes, title, NULL, NULL);
    if (!context->win) {
        err("%s", "Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(context->win);
    info("%s", "Main window created.");
    glfwWindowHint(GLFW_SAMPLES, 4);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        err("%s", "Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    info("%s", "GLEW initialized.");

    if (GLEW_KHR_debug) {
        int param = -1;
        glDebugMessageCallback(debugCB, &param);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    return context;
}

void contextErrorCB(int code, const char *msg) {
    err("[CONTEXT] %s (Code %i)", msg, code);
}

void resizeCB(GLFWwindow *win, int w, int h) {
    renderer->context->xRes = w;
    renderer->context->yRes = h;
    glViewport(0, 0, w, h);
}

void fps(double elapsed) {

    static int frames;
    static double freq = 0.0;

    if (freq > 0.5) {
        char title[25];
        snprintf(title, 25, "OpenGL @ %.2f", (double) frames / freq);
        glfwSetWindowTitle(renderer->context->win, title);
        frames = 0;
        freq = 0.0;
    }
    ++frames;
    freq += elapsed;
}

void debugCB(GLenum source, GLenum type, GLuint id, GLenum severity,
             GLsizei length, const GLchar *msg, const void *userParam) {

    int src_i = source - 0x8246;
    int type_i = type - 0x824C;
    int sev_i = severity - 0x9146;

    if (severity == 0x826B) {
        sev_i = 3;
    }

    debug(stderr, "[%s %s %s] (Id: %u)\n- .%s\n",
          ERR_SOURCE[src_i], ERR_TYPE[type_i], ERR_SEVERITY[sev_i], id, msg);
    fflush(stderr);
}