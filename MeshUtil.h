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
GLuint genVao(VaoType type, GLfloat length = 100000.0f, GLfloat texRes = 1000.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing inside */
GLuint cubeMapVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing outside */
GLuint cubeVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing up */
GLuint planeVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* terrain */
GLuint terrainVAO(GLfloat xlength = 100.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing outside */
GLuint sphereVAO(GLfloat radius = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

GLuint overlayVAO();
#endif	/* GLHELPER_H */

