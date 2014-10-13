#include "Mesh.h"
#include "ShaderCache.h"
#include "Camera.h"
#include "Debugger.h"

#include <string.h>

UniformVarFunc uniVarFuncLUT[] = {
    P, MV, MVP, MVPnoTrans
};

Mesh* newMesh() {
    Mesh* mesh = (Mesh*) malloc(sizeof (Mesh));

    return_guard(mesh, NULL);

    mesh->shaderProgram = glCreateProgram();
    check(mesh->shaderProgram, "Failed to create shader program");

    return mesh;
}

void freeMesh(Mesh* mesh) {

    glDeleteVertexArrays(1, &(mesh->vaoId));
    glDeleteProgram(mesh->shaderProgram);

    for (int i = 0; i < mesh->shadersLen; i++) {
        // already freed by freeCache()
        //free((char*) mesh->shaders[i]);
        mesh->shaders[i] = NULL;
    }

    free((char**) mesh->shaders);
    mesh->shaders = NULL;


    for (int i = 0; i < mesh->uniLen; i++) {
        free((char*) mesh->uniforms[i]);
        mesh->uniforms[i] = NULL;
    }

    free((char**) mesh->uniforms);
    mesh->uniforms = NULL;

    freeMaterial(mesh->mats);

    free(mesh->setUniformFunc);
    free(mesh);
}

/* draw functions */
void drawArrays(GLenum mode, GLint* first, GLsizei count) {
    glDrawArrays(mode, *first, count);
}

void drawElements(GLenum mode, GLint* first, GLsizei count) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, (GLvoid*) first);
}

/* uniform setter */
void P(GLint loc, Camera* cam) {
    
    //printMat(cam->perspective);

    glUniformMatrix4fv(loc, 1, GL_FALSE, cam->perspective);
}

void MV(GLint loc, Camera* cam) {

    mat4 orientation = cam->orientation;
    mat4 translation = cam->translation;

    GLfloat MVmat[16];
    mult(orientation, translation, MVmat);

    //printMat(orientation);
    //printMat(translation);
    //printMat(MVmat);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MVmat);
}

void MVP(GLint loc, Camera* cam) {
    mat4 P = cam->perspective;
    mat4 orientation = cam->orientation;
    mat4 translation = cam->translation;

    float MVP[16];
    mult(P, orientation, MVP);
    mult(MVP, translation, MVP);
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
}

void MVPnoTrans(GLint loc, Camera* cam) {

    mat4 P = cam->perspective;
    mat4 orientation = cam->orientation;

    float MVP[16];
    mult(P, orientation, MVP);
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
}
