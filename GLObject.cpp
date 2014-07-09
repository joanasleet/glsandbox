#include "GLObject.h"
#include "GLHelper.h"
#include "Logger.h"

GLObject::GLObject(GLenum bindTarget, bool doBind) : target(bindTarget) {

    switch (target) {
            // vertex array
        case GL_VERTEX_ARRAY:
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
        case GL_TEXTURE_CUBE_MAP:
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
            ERR("Unknown bind target (%d)", target);
            break;
    }
}

void GLObject::buffer(GLsizei size, GLvoid* data, GLenum usage) {
    glBufferData(target, size, (const GLvoid*) data, usage);
    catchError();
}

void GLObject::subBuffer(GLintptr offset, GLsizeiptr size, const GLvoid* data) {
    glBufferSubData(target, offset, size, data);
    catchError();
}

void GLObject::loadTexture(const char* file, GLenum slot, GLint mipmaps, GLint colorFormat, GLenum pixelFormat, GLenum pixelType) {

   
}

void GLObject::param(GLenum paramName, GLint paramValue) {
    glTexParameteri(target, paramName, paramValue);
}

void GLObject::param(GLenum paramName, GLfloat paramValue) {
    glTexParameterf(target, paramName, paramValue);
}