#ifndef GLCONTEXT_H
#define	GLCONTEXT_H

#include "common.h"

class GLContext {
public:
    GLFWwindow* main;

    int resX;
    int resY;

    GLContext(int resX = 800, int resY = 600, const char* title = "OpenGL");
    ~GLContext();

    void config();
    void printSpec();


private:

};

#endif	/* GLCONTEXT_H */

