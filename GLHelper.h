#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"

GLfloat* cube(float length, float midX, float midY, float midZ);

void vertexLayout(int position, int numberOfCoords);

GLuint loadTexture(const char* file);

#endif	/* GLHELPER_H */

