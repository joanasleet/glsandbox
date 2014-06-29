#include "GLObject.h"
#include "ErrorHandler.h"

GLObject::GLObject(GLenum bindTarget, bool doBind) {
    target = bindTarget;

    switch (target) {
            // vertex array
        case VERTEX_ARRAY_TARGET:
            glGenVertexArrays(1, &id);
            if (doBind) glBindVertexArray(id);
            break;
            // buffers
        case GL_ARRAY_BUFFER:
        case GL_ELEMENT_ARRAY_BUFFER:
            glGenBuffers(1, &id);
            if (doBind) glBindBuffer(target, id);
            break;
            // textures
        case GL_TEXTURE_1D:
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
            glGenTextures(1, &id);
            if (doBind) glBindTexture(target, id);
            break;
            // samplers
        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
            glGenSamplers(1, &id);
            if (doBind) glBindSampler(target, id);
            break;
        default:
            fprintf(stderr, "Unknown bind target (%d).\n", target);
            break;
    }
}

void GLObject::buffer(GLsizei size, GLvoid* data, GLenum usage) {
    glBufferData(target, size, data, usage);
    catchError();
}
