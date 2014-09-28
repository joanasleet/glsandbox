#ifndef CONTEXT_H
#define	CONTEXT_H

#include "common.h"

typedef struct Context {
    unsigned int xRes;
    unsigned int yRes;

    GLFWwindow* win;
} Context;

Context* newContext(unsigned int xRes = 1024, unsigned int yRes = 768, const char* title = "OpenGL");

void fps();
void resizeCB(GLFWwindow* win, int w, int h);
void contextErrorCB(int code, const char* msg);
void debugCB(GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar *msg, const void* userParam);


// should be part of the engine

double elapsedTime();
void resetTimer();

#endif	/* CONTEXT_H */

