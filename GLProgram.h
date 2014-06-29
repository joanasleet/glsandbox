#ifndef GLPROGRAM_H
#define	GLPROGRAM_H

#include "common.h"

class GLProgram {
public:
    GLuint id;

    GLProgram();
    ~GLProgram();

    void loadShader(const char* source, GLenum type);
    void setVar();
    GLuint getVar(const char* varName);
    void use();

private:
    char* bufferFile(const char* path);
};

#endif	/* GLPROGRAM_H */

