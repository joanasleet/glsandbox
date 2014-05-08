#ifndef GLCONTEXT_H
#define	GLCONTEXT_H

#include <cstdlib>
#include <stdio.h>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

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

