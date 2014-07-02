/* 
 * File:   common.h
 * Author: alex
 *
 * Created on June 29, 2014, 12:50 AM
 */

#ifndef COMMON_H
#define	COMMON_H

#include <cstdlib>
#include <stdio.h>

// GLEW
#include "/u/zimmeral/libs/glew-1.10.0/include/GL/glew.h"

// GLFW
#ifndef GLFW_DLL
#define GLFW_DLL
#endif
#include "/u/zimmeral/libs/glfw-3.0.4/include/GLFW/glfw3.h"

// Error Handling
#include "ErrorHandler.h"

// stb_image header only
#include "stb_image.h"

// openGL Math
#define GLM_FORCE_RADIANS

#include "/u/zimmeral/libs/glm/glm/glm.hpp"
#include "/u/zimmeral/libs/glm/glm/gtc/matrix_transform.hpp"
#include "/u/zimmeral/libs/glm/glm/gtc/type_ptr.hpp"

#endif	/* COMMON_H */

