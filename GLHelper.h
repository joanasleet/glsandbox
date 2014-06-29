#ifndef GLHELPER_H
#define	GLHELPER_H

#include <cstdlib>
#include <stdio.h>

#include <GL/glew.h>

GLfloat* cube(float length, float midX, float midY, float midZ);

void vertexLayout(int position, int numberOfCoords);

GLuint loadTexture(const char* file);

/* old stuff */
GLdouble* createArea(double minX, double maxX, double minY, double maxY, int resx, int resy);
void printArea(GLdouble* area, int size);
GLfloat* colorMap(float startR, float startG, float startB, float endR, float endG, float endB, int length);

#endif	/* GLHELPER_H */

