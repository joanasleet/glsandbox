#ifndef MESH_H
#define	MESH_H

#include "common.h"
#include "Texture.h"
#include "Camera.h"

typedef struct Mesh {
    GLuint vaoId;

    Texture* tex;

    const char** uniforms;
    void (**setUniformFunc)(GLint, Camera*);
    unsigned char uniLen;

    GLuint shaderProgram;
    const char** shaders;
    unsigned char shadersLen;

    void (*drawFunc)(GLenum mode, GLint* first, GLsizei count);
    GLenum mode;
    GLint first;
    GLsizei count;

} Mesh;

Mesh* newMesh(bool newVao = false);
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

