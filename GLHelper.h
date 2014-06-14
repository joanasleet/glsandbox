#ifndef GLHELPER_H
#define	GLHELPER_H

#include <cstdlib>
#include <stdio.h>

#include <GL/glew.h>

enum GEN_TYPE {
    VertexArray, Buffer
};

GLuint genId(enum GEN_TYPE type);
GLuint* genIds(enum GEN_TYPE type, int count);

GLdouble* createArea(double minX, double maxX, double minY, double maxY, int resx, int resy);
void printArea(GLdouble* area, int size);

GLfloat* colorMap(float startR, float startG, float startB, float endR, float endG, float endB, int length);


#endif	/* GLHELPER_H */

