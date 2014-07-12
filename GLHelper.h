#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"
#include "Object.h"

#define ABS(x) ((x > 0) ? x : -x)
#define MAX(a,b) ((a<b) ? b : a)
#define BUFFER_OFFSET(offset) ((void *) (offset))

/* facing inside */
Object* cubeMapVAO(GLfloat length = 1.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing outside */
Object* cubeVAO(GLfloat length = 1.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing up */
Object* planeVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

Object* cubeMapTex(const char* cubeFaces[], bool allSam = false);



#endif	/* GLHELPER_H */

