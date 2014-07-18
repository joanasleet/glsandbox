#include "Mesh.h"

extern ShaderCache* shaderCache;
extern UniformCache* uniformCache;

extern GLenum SHADER_TYPE[];

extern Camera* cam;

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

void preload(Mesh* mesh) {

    GLint prog = mesh->shaderProgram;

    /* cache shaders */
    for (int i = 0; i < mesh->shadersLen; ++i) {
        addShader(mesh->shaders[i], SHADER_TYPE[i], prog);
    }

    /* cache uniform locations */
    GLint loc;
    const char* str;
    const char* key;

    for (int i = 0; i < mesh->uniLen; ++i) {
        str = mesh->uniforms[i];
        loc = glGetUniformLocation(prog, str);
        key = getKey(str, prog);
        cache(uniformCache, key, loc);
    }
}

void render(Mesh* mesh) {

    GLint loc;
    glUseProgram(mesh->shaderProgram);

    for (int i = 0; i < mesh->uniLen; ++i) {
        loc = get(uniformCache, getKey(mesh->uniforms[i], mesh->shaderProgram));
        (*mesh->setUniformFunc)(loc);
    }

    bind(mesh->tex);
    glBindVertexArray(mesh->vaoId);
    (*mesh->drawFunc)(mesh->mode, &mesh->first, mesh->count);
}

/* draw functions */
void drawArrays(GLenum mode, GLint* first, GLsizei count) {
    glDrawArrays(mode, *first, count);
}

void drawElements(GLenum mode, GLint* first, GLsizei count) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, (GLvoid*) first);
}

/* uniform setter */
void P(GLint loc) {
    glm::mat4 P = *cam->perspective;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(P));
}

void MV(GLint loc) {
    glm::mat4 MV = *cam->modelview;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(MV));
}

void MVP(GLint loc) {
    glm::mat4 MV = *cam->modelview;
    glm::mat4 P = *cam->perspective;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(P*MV));
}