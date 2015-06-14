#ifndef INPUTMANAGER_H
#define	INPUTMANAGER_H

#include "common.h"

void cursorCB(GLFWwindow* win, double xpos, double ypos);

void cursorEnterCB(GLFWwindow* win, int enter);

void scrollCB(GLFWwindow* win, double xoffset, double yoffset);

void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods);

#endif


