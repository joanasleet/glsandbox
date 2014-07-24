#ifndef CONTEXT_H
#define	CONTEXT_H

#include "common.h"

#include "Camera.h"
#include "Texture.h"

typedef struct Context {
    unsigned int xRes;
    unsigned int yRes;

    GLFWwindow* win;
} Context;

Context* newContext(unsigned int xRes = 1024, unsigned int yRes = 768, const char* title = "OpenGL");

void config();
void fps();

void contextErrorCB(int code, const char* msg);
void resizeCB(GLFWwindow* win, int w, int h);

#endif	/* CONTEXT_H */

