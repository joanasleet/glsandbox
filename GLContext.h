#ifndef GLCONTEXT_H
#define	GLCONTEXT_H

#include "common.h"

#define BUFFER_OFFSET(offset) ((void *) (offset))

class GLContext {
public:
    GLFWwindow* main;
    
    int resX;
    int resY;

    GLContext(int resX = 800, int resY = 600, const char* title = "OpenGL");
    ~GLContext();
    
    void hideMouse();
    void printSpec();
    

private:
    
};

#endif	/* GLCONTEXT_H */

