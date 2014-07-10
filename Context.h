#ifndef CONTEXT_H
#define	CONTEXT_H

#include "common.h"

typedef struct Context {
    unsigned int xRes;
    unsigned int yRes;
    
    GLFWwindow* win;
} Context;

Context* createContext(unsigned int xRes = 1024, unsigned int yRes = 768, const char* title = "OpenGL");

void config();
void contextErrorCB(int code, const char* msg);

void fps();

#endif	/* CONTEXT_H */

