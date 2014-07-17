#ifndef MESH_H
#define	MESH_H

#include "common.h"
#include "Texture.h"
#include "ShaderCache.h"
#include "ShaderUtil.h"

typedef struct Mesh {
    GLuint vaoId;

    Texture* tex;

    const char* uniforms[];
    void (*setUniformFunc[])();
    unsigned char uniLen;

    GLuint shaderProgram;
    const char* shaders[];
    unsigned char shadersLen;

    void (*drawFunc)(GLenum mode, GLint* first, GLsizei count);
    GLenum mode;
    GLint first;
    GLsizei count;

} Mesh;

Mesh* newMesh(bool newVao = false);

void preRender(Mesh* mesh);
void render(Mesh* mesh);

/* draw functions */
void drawArrays(GLenum mode, GLint* first, GLsizei count);
void drawElements(GLenum mode, GLint* first, GLsizei count);

/* set uniforms functions */
void P();
void MV();
void MVP();


#endif	/* MESH_H */

