#include "Mesh.h"
#include "Debugger.h"

Mesh *newMesh() {

    Mesh *mesh = NEW(Mesh);
    return_guard(mesh, NULL);
    mesh->first = 0;
    return mesh;
}

void freeMesh(Mesh *mesh) {

    glDeleteVertexArrays(1, &(mesh->vaoId));
    free(mesh);
}

/* draw functions */
void drawArrays(GLenum mode, GLint *first, GLsizei count) {
    glDrawArrays(mode, *first, count);
}

void drawElements(GLenum mode, GLint *first, GLsizei count) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, (GLvoid *) first);
}