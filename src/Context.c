#include "Util.h"
#include "Camera.h"
#include "Engine.h"
#include "Context.h"
#include "LookupManager.h"

extern Engine *renderer;

Context *newContext(uint32 xRes, uint32 yRes, const char *title) {
    log_info("%s", "- - - - - - - Loading Context - - - - - - -");
    Context *context = (Context *) malloc(sizeof (Context));
    context->xRes = xRes;
    context->yRes = yRes;

    glfwSetErrorCallback(contextErrorCB);

    exit_guard(glfwInit());

    log_info("%s", "GLFW initialized.");

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    context->win = glfwCreateWindow(context->xRes, context->yRes, title, NULL, NULL);
    if (!context->win) {
        log_err("%s", "Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(context->win);
    log_info("%s", "Main window created.");
    glfwWindowHint(GLFW_SAMPLES, 16);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        log_err("%s", "Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    log_info("%s", "GLEW initialized.");

    if (GLEW_KHR_debug) {
        int param = -1;
        glDebugMessageCallback(debugCB, &param);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    GLint texSlots = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texSlots);
    log_info("Max texture slots: %d", texSlots);

    return context;
}

void contextErrorCB(int code, const char *msg) {
    log_err("[CONTEXT] %s (Code %i)", msg, code);
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
        snprintf( title, 25, "OpenGL @ %.2f", (double) frames / freq );
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

    log_warn("[%s %s %s] (id: %u)\n\"%s\"",
             ERR_SOURCE[src_i], ERR_TYPE[type_i], ERR_SEVERITY[sev_i], id, msg);
    fflush(stderr);
}
