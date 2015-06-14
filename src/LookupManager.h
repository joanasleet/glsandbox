#ifndef LOOKUPMANAGER_H
#define LOOKUPMANAGER_H

#include "common.h"
#include "Shader.h"

/* debug */
extern const char *ERR_TYPE[];
extern const char *ERR_SOURCE[];
extern const char *ERR_SEVERITY[];

/* GL */
extern GLenum ShaderType[];

/* uniforms */
extern UniformSetter UniVarFuncs[];

/* draw call */
extern GLuint DrawMode[];
extern enum { PLANE, CUBE, SPHERE, CUBEMAP, OVERLAY, TERRAIN, SKYDOME, SCREEN_QUAD } VaoType;

#endif
