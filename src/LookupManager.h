#ifndef LOOKUPMANAGER_H
#define LOOKUPMANAGER_H

#include "Mesh.h"
#include "common.h"
#include "Shader.h"
#include "Newton/include/Newton.h"

/* debug */
extern const char *ERR_TYPE[];
extern const char *ERR_SOURCE[];
extern const char *ERR_SEVERITY[];

/* GL */
extern GLenum ShaderType[];
extern const char* ShaderName[];

/* uniforms */
extern UniformSetter UniVarFuncs[];

/* vao generator */
extern VaoFunc VaoFuncs[];

/* draw call */
extern GLuint DrawMode[];
extern DrawFunc DrawFuncs[];

/* 
 * physics lookup */

typedef NewtonBody* (*PhysicsBodyCF)(const NewtonWorld* const, const NewtonCollision* const, const dFloat* const);
extern PhysicsBodyCF PhysicsBodyFuncs[];

#endif

