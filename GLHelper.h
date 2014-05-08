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

GLfloat* createArea(float minX, float maxX, float minY, float maxY, int n);
void printArea(GLfloat* area, int size);

GLfloat* colorMap(float startR, float startG, float startB, float endR, float endG, float endB, int length);


#endif	/* GLHELPER_H */

