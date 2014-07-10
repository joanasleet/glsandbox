#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"
#include "Object.h"

#define ABS(x) ((x > 0) ? x : -x)
#define BUFFER_OFFSET(offset) ((void *) (offset))

Object* cubeVAO(GLfloat length = 1.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);
Object* planeVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

Object* cubeMap(const char* cubeFaces[]);



#endif	/* GLHELPER_H */

