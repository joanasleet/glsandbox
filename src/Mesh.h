#ifndef MESH_H
#define	MESH_H

#include "common.h"
#include "Texture.h"
#include "Camera.h"

typedef void (*UniformVarFunc)(GLint, Camera*);
typedef void (*DrawFunc)(GLenum, GLint*, GLsizei);

typedef struct {
    GLuint vaoId;

    const char* *uniforms;
    UniformVarFunc *setUniformFunc;
    uint8 uniLen;

    GLuint shaderProgram;
    const char* *shaders;
    uint8 shadersLen;

    DrawFunc drawFunc;
    GLenum mode;
    GLint first;
    GLsizei count;

    Material* mats;

} Mesh;

Mesh* newMesh();
void freeMesh(Mesh* mesh);

/* draw functions */
void drawArrays(GLenum mode, GLint* first, GLsizei count);
void drawElements(GLenum mode, GLint* first, GLsizei count);

/* set uniforms functions */
void P(GLint loc, Camera* cam);
void MV(GLint loc, Camera* cam);
void MVP(GLint loc, Camera* cam);
void MVPnoTrans(GLint loc, Camera* cam);

#endif	/* MESH_H */

