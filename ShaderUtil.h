#ifndef SHADER_UTIL_H
#define	SHADER_UTIL_H

#include "common.h"
#include "ShaderCache.h"

extern GLenum SHADER_TYPE[];

void addShader(char* source, GLenum type, GLuint prog);
GLuint compileShader(char* source, GLenum type);

char* bufferFile(const char* path);

#endif	/* SHADER_H */

