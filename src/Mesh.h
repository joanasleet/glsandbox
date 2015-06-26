#ifndef MESH_H
#define MESH_H

#include "common.h"

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

    GLuint vaoId;
    GLuint vboId;

    GLenum mode;
    GLint first;
    GLsizei count;
    DrawFunc draw;

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
void cubeVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void planeVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void circleVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void sphereVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void terrainVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void overlayVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void cubeMapVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );
void screenQuadVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );

/* outlier - needs integration */
void staticTextVAO( const char* text, GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh );

#endif

