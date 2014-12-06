#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <math.h>

/* GLEW */
#include <GL/glew.h>

/* GLFW */
#ifndef GLFW_DLL
#define GLFW_DLL
#endif
#include <GLFW/glfw3.h>

#define ABS(x) ((x > 0) ? x : -x)
#define MAX(a,b) ((a<b) ? b : a)

#define TRUE 1
#define FALSE 0

#define NEW(type) ((type*) malloc(sizeof(type)))

/*
 * unsigned number types */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

/*
 * signed number types */
typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long int64;

#endif  /* COMMON_H */

