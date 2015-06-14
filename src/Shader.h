#ifndef SHADER_H
#define SHADER_H

#include "common.h"

#include "State.h"
#include "Camera.h"

typedef void (*UniformSetter)(GLint, Camera *, State *, double);

typedef struct {

    GLuint program;

    uint8 stageCount;
    const char **stages;

    uint8 uniformCount;
    UniformSetter *setters;
    const char **uniforms;

} Shader;

Shader *newShader();
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

#endif
