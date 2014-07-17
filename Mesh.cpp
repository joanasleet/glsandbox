#include "Mesh.h"

extern ShaderCache* shaderCache;
extern UniformCache* uniformsCache;

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

void preRender(Mesh* mesh) {

    
    
}

void render(Mesh* mesh) {

    glUseProgram(mesh->shaderProgram);

    char* key;
    const char* uniform;
    const char** uniforms = mesh->uniforms;


    int n = sizeof (uniforms) / sizeof (uniforms[0]);
    int digits = floor(log10(abs(mesh->shaderProgram))) + 1;
    for (int i = 0; i < n; ++i) {
        uniform = uniforms[i];
        key = (char*) malloc(sizeof (char)*(strlen(uniform) + digits));
        sprintf(key, "%i%s", mesh->shaderProgram, uniform);
        
        free(key);
    }

    bind(mesh->tex);
    glBindVertexArray(mesh->vaoId);
    (*mesh->drawFunc)(mesh->mode, &mesh->first, mesh->count);
}

void drawArrays(GLenum mode, GLint* first, GLsizei count) {
    glDrawArrays(mode, *first, count);
}

void drawElements(GLenum mode, GLint* first, GLsizei count) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, (GLvoid*) first);
}