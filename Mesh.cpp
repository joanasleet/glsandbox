#include "Mesh.h"
#include "ShaderCache.h"
#include "Camera.h"
#include "Logger.h"

#include <string.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Mesh* newMesh(bool newVao) {
    Mesh* mesh = (Mesh*) malloc(sizeof (Mesh));
    if (!mesh) {
        ERR("Failed to allocate mesh");
        return NULL;
    }

    if (newVao) {
        glGenVertexArrays(1, &mesh->vaoId);
        glBindVertexArray(mesh->vaoId);
    }

    mesh->shaderProgram = glCreateProgram();
    if (!mesh->shaderProgram) {
        ERR("Failed to create shader program");
    }

    return mesh;
}

/* draw functions */
void drawArrays(GLenum mode, GLint* first, GLsizei count) {
    //INFO("Calling:\tdrawArrays(%d, %d, %d)", mode, *first, count);
    glDrawArrays(mode, *first, count);
}

void drawElements(GLenum mode, GLint* first, GLsizei count) {
    //INFO("Calling:\tdrawElements(%d, %d, GL_UNSIGNED_INT, %d)", mode, count, *first);
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
    //INFO("Calling P(%d)", loc);
    glm::mat4 P = *(cam->perspective);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(P));
}

void MV(GLint loc, Camera* cam) {
    //INFO("Calling MV(%d)", loc);
    glm::mat4 orientation = *(cam->orientation);
    glm::mat4 translation = *(cam->translation);
    glm::mat4 MV = orientation * translation;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MV));
}

void MVP(GLint loc, Camera* cam) {
    //INFO("Calling MVP(%d)", loc);
    glm::mat4 P = *(cam->perspective);
    glm::mat4 orientation = *(cam->orientation);
    glm::mat4 translation = *(cam->translation);
    glm::mat4 MVP = P * orientation * translation;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));
}

void MVPnoTrans(GLint loc, Camera* cam) {
    //INFO("Calling MVPnoTrans(%d)", loc);
    glm::mat4 P = *(cam->perspective);
    glm::mat4 orientation = *(cam->orientation);
    glm::mat4 MVP = P * orientation;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MVP));
}
