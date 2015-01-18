#ifndef SHADER_H
#define SHADER_H

#include "common.h"

#include "State.h"
#include "Camera.h"

typedef void (*UniformSetter)();

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
void P(GLint loc, Camera *cam, State *objState);
void MV(GLint loc, Camera *cam, State *objState);
void MVP(GLint loc, Camera *cam, State *objState);
void MVPnoTrans(GLint loc, Camera *cam, State *objState);
void CamPos(GLint loc, Camera *cam, State *objState);

/* object specific */
void objMV(GLint loc, Camera *cam, State *objState);
void objMVnoTrans(GLint loc, Camera *cam, State *objState);

/* misc */
void gTime(GLint loc, float _time);

#endif