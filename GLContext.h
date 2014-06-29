#ifndef GLCONTEXT_H
#define	GLCONTEXT_H

#include "common.h"

#define BUFFER_OFFSET(offset) ((void *) (offset))

class GLContext {
public:
    GLFWwindow* main;

    GLContext(int resX = 800, int resY = 600, const char* title = "OpenGL");
    ~GLContext();

    void setRes(int x, int y);
    void printSpec();

private:
    int resX;
    int resY;
};

#endif	/* GLCONTEXT_H */

