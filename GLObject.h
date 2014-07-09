#ifndef GLOBJECT_H
#define	GLOBJECT_H

#include "common.h"

class GLObject {
public:
    GLuint id;
    GLenum target;

    GLObject(GLenum bindTarget, bool doBind = true);
    ~GLObject();

    void bind();

    // buffer specific
    void buffer(GLsizei size, GLvoid* data, GLenum usage = GL_STATIC_DRAW);
    void subBuffer(GLintptr offset, GLsizeiptr size, const GLvoid* data);

    // texture specific
    void loadTexture(const char* file, GLenum slot = GL_TEXTURE0, GLint mipmaps = 0,
            GLint colorFormat = GL_RGBA, GLenum pixelFormat = GL_RGBA,
            GLenum pixelType = GL_UNSIGNED_BYTE);

    void param(GLenum paramName, GLint paramValue);
    void param(GLenum paramName, GLfloat paramValue);

private:

};

#endif	/* GLOBJECT_H */

