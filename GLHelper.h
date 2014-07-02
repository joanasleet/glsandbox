#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"

unsigned char* texture(const char* file, int* width, int* height, int* compression, int channels = 4);

GLfloat* cube(float length, float midX, float midY, float midZ);

extern void setGLStates();
extern void setupMVP();

#endif	/* GLHELPER_H */

