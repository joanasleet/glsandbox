#ifndef SHADER_H
#define SHADER_H

#include "common.h"

#include "State.h"
#include "Camera.h"
#include "LuaScript.h"

typedef void (*UniformSetter)(GLint, Camera *, State *, double);

typedef struct {

    Cache *uniforms;
    GLuint program;

    uint8 _pad[4];

} Shader;

/* main */
Shader *newShader();
void addShader(const char* source, GLenum type, GLuint prog, Cache* cache);
GLuint compileShader(const char* source, GLuint shaderId);
void freeShader(Shader *shader);

/* uniforms setter */
void P(GLint loc, Camera *cam, State *objState, double globalTime);
void MV(GLint loc, Camera *cam, State *objState, double globalTime);
void MVnoTrans(GLint loc, Camera *cam, State *objState, double globalTime);
void MVP(GLint loc, Camera *cam, State *objState, double globalTime);
void MVPnoTrans(GLint loc, Camera *cam, State *objState, double globalTime);
void camPos(GLint loc, Camera *cam, State *objState, double globalTime);

/* object specific */
void objMV(GLint loc, Camera *cam, State *objState, double globalTime);
void objMVnoTrans(GLint loc, Camera *cam, State *objState, double globalTime);

/* misc */
void gTime(GLint loc, Camera *cam, State *objState, double globalTime);

/* helper */
char* bufferFile(const char* path);

#endif

