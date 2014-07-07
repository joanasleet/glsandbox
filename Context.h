#ifndef CONTEXT_H
#define	CONTEXT_H

#include "common.h"

typedef struct Context {
    unsigned int xRes;
    unsigned int yRes;
    
    GLFWwindow* win;
} Context;

Context* createContext(unsigned int xRes = 800, unsigned int yRes = 600, const char* title = "OpenGL");

#endif	/* CONTEXT_H */

