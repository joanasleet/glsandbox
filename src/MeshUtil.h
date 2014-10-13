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


/* 
 * Indices into LUT for draw parameters */
typedef enum {
    PLANE, CUBE, SPHERE, CUBEMAP, OVERLAY, TERRAIN
} VaoType;

/* 
 * LUT for draw parameters */
extern GLuint DrawMode[];
extern GLsizei VertexCount[];

/* vao factory */
GLuint genVao(VaoType type, GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ);

/* facing inside */
GLuint cubeMapVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ);

/* facing outside */
GLuint cubeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ);

/* facing up */
GLuint planeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ);

/* terrain */
GLuint terrainVAO(GLfloat xlength, GLfloat midX, GLfloat midY, GLfloat midZ);

/* facing outside */
GLuint sphereVAO(GLfloat radius, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ);

GLuint overlayVAO();
#endif	/* GLHELPER_H */

