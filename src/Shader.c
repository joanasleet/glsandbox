#include "Util.h"
#include "Shader.h"
#include "MatrixMath.h"

Shader *newShader() {

    Shader *shader = alloc( Shader, 1 );
    shader->program = glCreateProgram();
    return shader;
}

void freeShader(Shader *shader) {

    glDeleteProgram(shader->program);

    for( int i=0; i<shader->stageCount; ++i )
        free( ( char* ) shader->stages[i] );

    free(shader->stages);

    for( int i=0; i<shader->uniformCount; ++i )
        free( ( char* ) shader->uniforms[i] );

    free(shader->uniforms);
    free(shader->setters);

    free(shader);
}

/* uniform setter */
void P(GLint loc, Camera *cam, State *objState, double globalTime) {

    glUniformMatrix4fv(loc, 1, GL_FALSE, cam->perspective);
}

void MV(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *camState = cam->state;

    // orientation
    float rotaQ[4];
    rotate3D(rotaQ, camState->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    // translation
    GLfloat translation[16];
    translate(translation, -camState->position[0], -camState->position[1], -camState->position[2]);

    // ModelView
    GLfloat MVmat[16];
    mult(orientation, translation, MVmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVmat);
}

void MVnoTrans(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *camState = cam->state;

    // orientation
    float rotaQ[4];
    rotate3D(rotaQ, camState->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, orientation);
}

void MVP(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *state = cam->state;

    // orientation
    float rotaQ[4];
    rotate3D(rotaQ, state->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    // translation
    GLfloat translation[16];
    translate(translation, -state->position[0], -state->position[1], -state->position[2]);

    GLfloat temp[16];
    mult(cam->perspective, orientation, temp);
    
    // (perspective * orientation) * translation
    GLfloat MVPmat[16];
    mult(temp, translation, MVPmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVPmat);
}

void MVPnoTrans(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *camState = cam->state;

    // orientation
    float rotaQ[4];
    rotate3D(rotaQ, camState->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    // ModelViewPerspective
    float MVP[16];
    mult(cam->perspective, orientation, MVP);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
}

// buggy stuff here

void objMV(GLint loc, Camera *cam, State *objState, double globalTime) {

    float rotaQ[4];
    rotate3D(rotaQ, cam->state->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    float translation[16];
    vec3 pos = objState->position;
    translate(translation, pos[0], pos[1], pos[2]);

    float MV[16];
    mult(orientation, translation, MV);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MV);
}

void objMVnoTrans(GLint loc, Camera *cam, State *objState, double globalTime) {

    float rotaQ[4];
    rotate3D(rotaQ, cam->state->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    glUniformMatrix4fv(loc, 1, GL_FALSE, orientation);
}

void camPos(GLint loc, Camera *cam, State *objState, double globalTime) {
    glUniform3fv(loc, 1, cam->state->position);
}

void gTime(GLint loc, Camera *cam, State *objState, double globalTime) {
    glUniform1f(loc, globalTime);
}
