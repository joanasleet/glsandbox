#ifndef COMMON_H
#define	COMMON_H

#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <cstdarg>

// GLEW
#include <GL/glew.h>

// GLFW
#ifndef GLFW_DLL
#define GLFW_DLL
#endif
#include <GLFW/glfw3.h>

// Error Handling
#define LOG_TO_FILE

// stb_image header only
#include "stb_image.h"

// openGL Math
#define GLM_FORCE_RADIANS

#define ABS(x) ((x > 0) ? x : -x)
#define MAX(a,b) ((a<b) ? b : a)

#endif	/* COMMON_H */

