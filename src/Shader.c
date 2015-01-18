#include "Shader.h"
#include "MatrixMath.h"

Shader *newShader() {

    Shader *shader = NEW(Shader);
    shader->program = glCreateProgram();
    return shader;
}

void freeShader(Shader *shader) {

    glDeleteProgram(shader->program);
    free(shader->stages);
    free(shader->setters);
    free(shader->uniforms);

    free(shader);
}

/* uniform setter */
void P(GLint loc, Camera *cam, State *objState) {

    glUniformMatrix4fv(loc, 1, GL_FALSE, cam->perspective);
}

void MV(GLint loc, Camera *cam, State *objState) {

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

void MVP(GLint loc, Camera *cam, State *objState) {

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

void MVPnoTrans(GLint loc, Camera *cam, State *objState) {

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

void objMV(GLint loc, Camera *cam, State *objState) {

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

void objMVnoTrans(GLint loc, Camera *cam, State *objState) {

    float rotaQ[4];
    rotate3D(rotaQ, cam->state->angles);
    invertQ(rotaQ);

    GLfloat orientation[16];
    rotateQ(orientation, rotaQ);

    glUniformMatrix4fv(loc, 1, GL_FALSE, orientation);
}

void gTime(GLint loc, GLfloat _time) {
    glUniform1f(loc, _time);
}

void CamPos(GLint loc, Camera *cam, State *objState) {
    glUniform3fv(loc, 1, cam->state->position);
}