#ifndef MESH_H
#define MESH_H

#include "common.h"

#include "Newton/include/Newton.h"

#define BUFFER_OFFSET(offset) ((void *) (offset))

#define VAO(name) \
    GLuint name; \
    glGenVertexArrays(1, &name); \
    glBindVertexArray(name); \
     
#define VBO(name, target) \
    GLuint name; \
    glGenBuffers(1, &name); \
    glBindBuffer(target, name); \
     
/* draw function prototype */
typedef void (*DrawFunc)(GLenum, GLint *, GLsizei);

typedef struct {

    NewtonBody *nbody;
    NewtonCollision *ncol;
    DrawFunc draw;

    GLuint vaoId;
    GLuint vboId;
    GLuint eabId;
    GLuint fboId;

    GLenum mode;
    GLint first;
    GLsizei count;

    uint8 _pad[4];
} Mesh;

/* vao generator function prototype */
typedef void ( *VaoFunc )( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );

/* creation stuff */
Mesh *newMesh();
void freeMesh(Mesh *mesh);

/* draw functions */
void drawArrays(GLenum mode, GLint *first, GLsizei count);
void drawElements(GLenum mode, GLint *first, GLsizei count);

/* vao generators */
void planeVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void cubeInVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void cubeOutVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void sphereInVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void sphereOutVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void terrainVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void overlayVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );

/* outlier - needs integration */
void staticTextVAO( const char* text, GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );

#endif

