#ifndef MESH_H
#define MESH_H

#include "common.h"

typedef void (*DrawFunc)(GLenum, GLint *, GLsizei);
typedef void (*PreDrawFunc)();

typedef struct {

    GLuint vaoId;

    GLenum mode;
    GLint first;
    GLsizei count;
    DrawFunc draw;

} Mesh;

Mesh *newMesh();
void freeMesh(Mesh *mesh);

/* draw functions */
void drawArrays(GLenum mode, GLint *first, GLsizei count);
void drawElements(GLenum mode, GLint *first, GLsizei count);

#endif