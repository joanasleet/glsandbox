#ifndef GLOBJECT_H
#define	GLOBJECT_H

#include <stdio.h>
#include <cstdlib>

#include <GL/glew.h>

#define VERTEX_ARRAY_TARGET -1

class GLObject {
public:
    GLuint id;
    GLenum target;

    GLObject(GLenum bindTarget, bool doBind = true);
    ~GLObject();

    void buffer(GLsizei size, GLvoid* data, GLenum usage);

private:
    
};

#endif	/* GLOBJECT_H */

