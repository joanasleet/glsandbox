#ifndef GLPROGRAM_H
#define	GLPROGRAM_H

#include <stdio.h>
#include <cstdlib>

#include <GL/glew.h>

class GLProgram {
public:
    GLuint id;

    GLProgram();
    ~GLProgram();

    void loadShader(const char* source, GLenum type);
    void use();

private:
    char* bufferFile(const char* path);
};

#endif	/* GLPROGRAM_H */

