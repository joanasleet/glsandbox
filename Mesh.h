#ifndef MESH_H
#define	MESH_H

#include "common.h"
#include "Texture.h"
#include "ShaderCache.h"
#include "ShaderUtil.h"

typedef struct Mesh {
    GLuint vaoId;

    Texture* tex;

    const char** uniforms;
    void (**setUniformFunc)(GLint);
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

void preload(Mesh* mesh);
void render(Mesh* mesh);

/* draw functions */
void drawArrays(GLenum mode, GLint* first, GLsizei count);
void drawElements(GLenum mode, GLint* first, GLsizei count);

/* set uniforms functions */
void P(GLint loc);
void MV(GLint loc);
void MVP(GLint loc);


#endif	/* MESH_H */

