#include "Context.h"
#include "Camera.h"
#include "Debugger.h"
#include "Engine.h"

extern Engine* renderer;

Context* createContext(uint32 xRes, uint32 yRes, const char *title) {
    info("%s","–––––––––––––––– Log Start ––––––––––––––––––");
    Context* context = (Context*) malloc(sizeof (Context));
    context->xRes = xRes;
    context->yRes = yRes;

    glfwSetErrorCallback(contextErrorCB);

    exit_guard(glfwInit());

    info("%s","GLFW initialized.");

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    context->win = glfwCreateWindow(context->xRes, context->yRes, title, NULL, NULL);
    if (!context->win) {
        err("%s","Failed to create main window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(context->win);
    info("%s","Main window created.");
    glfwWindowHint(GLFW_SAMPLES, 4);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        err("%s","Failed to start GLEW.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    info("%s","GLEW initialized.");

    if (GLEW_KHR_debug) {
        int param = -1;
        glDebugMessageCallback(debugCB, &param);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    return context;
}

void contextErrorCB(int code, const char* msg) {
    err("[CONTEXT] %s (Code %i)", msg, code);
}

void resizeCB(GLFWwindow* win, int w, int h) {
    renderer->glContext->xRes = w;
    renderer->glContext->yRes = h;
    glViewport(0, 0, w, h);
}

void fps() {
    static double prevTime = 0;
    static int frames = 0;

    double currTime = glfwGetTime();
    double diffTime = currTime - prevTime;
    
    if (diffTime > 0.5) {
        prevTime = currTime;
        char title[20];
        sprintf(title, "OpenGL @ %.2f", (double) frames / diffTime);
        glfwSetWindowTitle(renderer->glContext->win, title);
        frames = 0;
    }
    ++frames;
}

/*
double elapsedTime() {
    static double startTime = glfwGetTime();
    return (glfwGetTime() - startTime);
}

void resetTimer() {
    glfwSetTime(0.0);
} */

/* where to put this shit ? */
const char* SEVERITY_LVL[] = {
    "HIGH",
    "MEDIUM",
    "LOW",
    "NOTE"
};

const char* SOURCE[] = {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
};

const char* TYPE[] = {
    "ERROR",
    "DEPRECATED BEHAVIOUR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
};

void debugCB(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar *msg, const void* userParam) {

    int src_i = source - 0x8246;
    int type_i = type - 0x824C;
    int sev_i = severity - 0x9146;

    if (severity == 0x826B) {
        sev_i = 3;
    }

    debug(stderr, "[%s %s %s] (Id: %u)\n- .%s\n",
            SOURCE[src_i], TYPE[type_i], SEVERITY_LVL[sev_i], id, msg);
    fflush(stderr);
}