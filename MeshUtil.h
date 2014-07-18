#ifndef GLHELPER_H
#define	GLHELPER_H

#include "common.h"
#include "Texture.h"
#include "Mesh.h"

#define BUFFER_OFFSET(offset) ((void *) (offset))

#define VAO(name) \
GLuint name; \
glGenVertexArrays(1, &name); \
glBindVertexArray(name); \

#define VBO(name, target) \
GLuint name; \
glGenBuffers(1, &name); \
glBindBuffer(target, name); \

#define MAX_MESHES 10
extern Mesh* meshes[MAX_MESHES];
extern int nextMeshSlot;

#define ADD_MESH(mesh) \
do { \
    if (nextMeshSlot > MAX_MESHES) { \
        ERR("Max meshes reached"); \
        break; \
    } \
    meshes[nextMeshSlot++] = mesh;\
} while (0);\



/* facing inside */
GLuint cubeMapVAO(GLfloat length = 1.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing outside */
GLuint cubeVAO(GLfloat length = 1.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

/* facing up */
GLuint planeVAO(GLfloat length = 100.0f, GLfloat texRes = 1.0f, GLfloat midX = 0.0f, GLfloat midY = 0.0f, GLfloat midZ = 0.0f);

#endif	/* GLHELPER_H */

