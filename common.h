#ifndef COMMON_H
#define	COMMON_H

#include <cstdlib>
#include <stdio.h>

// GLEW
#include <GL/glew.h>

// GLFW
#ifndef GLFW_DLL
#define GLFW_DLL
#endif
#include <GLFW/glfw3.h>

// Error Handling
#include "ErrorHandler.h"

// stb_image header only
#include "stb_image.h"

// openGL Math
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#endif	/* COMMON_H */

