#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"
#include "Object.h"

#define BUFFER_OFFSET(offset) ((void *) (offset))

GLfloat* cubeVD(GLsizei* size, GLfloat length = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f, bool genTexels = true);
GLfloat* planeVD(GLsizei* size, GLfloat length = 100.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f, bool genTexels = true);

Object* cubeVAO(GLfloat length = 1.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);
Object* planeVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);



#endif	/* GLHELPER_H */

