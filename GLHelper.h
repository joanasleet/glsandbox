#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"

unsigned char* texture(const char* file, int* width, int* height, int* compression, int channels = 4);

GLfloat* cubeVD(GLsizei* size, GLfloat length = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.5f, GLfloat midZ = 0.0f, bool genTexels = true);
GLfloat* planeVD(GLsizei* size, GLfloat length = 100.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f, bool genTexels = true);



#endif	/* GLHELPER_H */

