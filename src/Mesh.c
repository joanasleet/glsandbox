#include "Mesh.h"
#include "ShaderCache.h"
#include "Camera.h"
#include "Debugger.h"

#include <string.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

UniformVarFunc uniVarFuncLUT[] = {
    P, MV, MVP, MVPnoTrans
};

Mesh* newMesh() {
    Mesh* mesh = (Mesh*) malloc(sizeof (Mesh));

    return_guard(mesh, NULL);
    /*if (!mesh) {
        err("Failed to allocate mesh");
        return NULL;
    }*/

    mesh->shaderProgram = glCreateProgram();
    check(mesh->shaderProgram, "Failed to create shader program");
    /*if (!mesh->shaderProgram) {
        err("Failed to create shader program");
    }*/

    return mesh;
}

// TODO: implement freeMesh

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

/* helper */
void printMatrix(float* m) {
    printf("- - - - - - - - - - - - - -\n");
    for (int i = 0; i < 4; ++i) {
        printf("%.1f\t%.1f\t%.1f\t%.1f\n", m[i * 4], m[i * 4 + 1], m[i * 4 + 2], m[i * 4 + 3]);
    }
}

/* uniform setter */
void P(GLint loc, Camera* cam) {
    glm::mat4 P = *(cam->perspective);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(P));
}

void MV(GLint loc, Camera* cam) {
    glm::mat4 orientation = *(cam->orientation);
    glm::mat4 translation = *(cam->translation);
    glm::mat4 MV = orientation * translation;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MV));
}

void MVP(GLint loc, Camera* cam) {
    glm::mat4 P = *(cam->perspective);
    glm::mat4 orientation = *(cam->orientation);
    glm::mat4 translation = *(cam->translation);
    glm::mat4 MVP = P * orientation * translation;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));
}

void MVPnoTrans(GLint loc, Camera* cam) {
    glm::mat4 P = *(cam->perspective);
    glm::mat4 orientation = *(cam->orientation);
    glm::mat4 MVP = P * orientation;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));
}
