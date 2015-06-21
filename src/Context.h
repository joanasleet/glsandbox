#ifndef CONTEXT_H
#define	CONTEXT_H

#include "common.h"

typedef struct {
    uint32 xRes;
    uint32 yRes;

    GLFWwindow* win;
} Context;

Context* newContext(uint32 xres, uint32 yres, const char* title);

void fps(double elapsed);
void resizeCB(GLFWwindow* win, int w, int h);
void contextErrorCB(int code, const char* msg);
void debugCB(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar *msg, const void* userParam);

#endif	/* CONTEXT_H */
