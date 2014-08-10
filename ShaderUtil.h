#ifndef SHADER_UTIL_H
#define	SHADER_UTIL_H

#include "common.h"
#include "ShaderCache.h"

void addShader(const char* source, GLenum type, GLuint prog, ShaderCache* cache);
GLuint compileShader(const char* source, GLenum type);

char* bufferFile(const char* path);

#endif	/* SHADER_H */

