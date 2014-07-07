#ifndef SHADER_PROGRAM_H
#define	SHADER_PROGRAM_H

#include "common.h"

void loadShader(const char* source, GLenum type, GLuint prog);

char* bufferFile(const char* path);

#endif	/* SHADER_H */

