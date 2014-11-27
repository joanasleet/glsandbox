#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"
#include "Deallocator.h"

#define BUFFER_OFFSET(offset) ((void *) (offset))

#define VAO(name) \
GLuint name; \
glGenVertexArrays(1, &name); \
glBindVertexArray(name); \

#define VBO(name, target) \
GLuint name; \
glGenBuffers(1, &name); \
glBindBuffer(target, name); \
storeVbo(name); \

/* vao factory */
GLuint genVao(uint32 type, GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);

/* facing inside */
GLuint cubeMapVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);

/* facing outside */
GLuint cubeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);

/* facing up */
GLuint planeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);

/* terrain */
GLuint terrainVAO(GLfloat xlength, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);

/* facing outside */
GLuint circleVAO(GLfloat radius, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);
GLuint sphereVAO(GLfloat radius, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32* vertcount);

GLuint staticTextVAO(const char* text, GLfloat size, GLfloat x, GLfloat y, int32* vertcount);

GLuint overlayVAO(int32* vertcount);

#endif	/* GLHELPER_H */

