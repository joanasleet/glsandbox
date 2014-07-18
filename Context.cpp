#include "Context.h"

extern Context* context;
extern Camera* cam;

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

void config() {

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5, 0.5, 0.7, 1.0);
    glfwSetInputMode(context->win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorEnterCallback(context->win, cursorEnterCB);
    glfwSetScrollCallback(context->win, scrollCB);
    glfwSetKeyCallback(context->win, keyCB);
    glfwSetWindowSizeCallback(context->win, resizeCB);

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexSlots);
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
        glfwSetWindowTitle(context->win, title);
        frames = 0;
    }
    ++frames;
}

void contextErrorCB(int code, const char* msg) {
    ERR("In context - %s (Code %i)", msg, code);
}

void resizeCB(GLFWwindow* win, int w, int h) {
    context->xRes = w;
    context->yRes = h;
    glViewport(0, 0, w, h);
}

void printWatchLog() {
    watchLog = fopen(WATCH_LOG_NAME, "a+");
    if (!watchLog) {
        ERR("Failed to open log <%s>", WATCH_LOG_NAME);
        return;
    }

    fprintf(watchLog, "–––––––––––––– Watch Log ––––––––––––––\n");
    fprintf(watchLog, "DIRECTION:\n\t%.1f  %.1f  %.1f\n", cam->dirX, cam->dirY, cam->dirZ);
    fprintf(watchLog, "POSITION:\n\t%.1f  %.1f  %.1f\n", cam->xPos, cam->yPos, cam->zPos);
    fprintf(watchLog, "SPEED:\n\t%.1f\n", cam->defaultSpeed);

    fclose(watchLog);
}