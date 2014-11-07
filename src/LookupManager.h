#ifndef LOOKUPMANAGER_H
#define	LOOKUPMANAGER_H

#include "common.h"
#include "Mesh.h"

/* debug */
extern const char* ERR_TYPE[];
extern const char* ERR_SOURCE[];
extern const char* ERR_SEVERITY[];

/* GL */
extern GLenum ShaderType[];

/* uniforms */
extern UniformVarFunc UniVarFuncs[];

/* draw call */
extern GLuint DrawMode[];
extern GLsizei VertexCount[];
extern enum { PLANE, CUBE, SPHERE, CUBEMAP, OVERLAY, TERRAIN } VaoType;

#endif