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
    GLfloat orientation[16];
    rotate3D(orientation, camState->angles);

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
    GLfloat orientation[16];
    state->angles[0] *= -1.0f;
    state->angles[1] *= -1.0f;
    state->angles[2] *= -1.0f;
    rotate3D(orientation, state->angles);

    // translation
    GLfloat translation[16];
    translate(translation, -state->position[0], -state->position[1], -state->position[2]);

    // perspective * orientation
    GLfloat VPmat[16];
    mult(cam->perspective, orientation, VPmat);

    // (perspective * orientation) * translation
    GLfloat MVPmat[16];
    mult(VPmat, translation, MVPmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVPmat);
}

void MVPnoTrans(GLint loc, Camera *cam, State *objState) {

    // orientation
    GLfloat orientation[16];
    rotate3D(orientation, cam->state->angles);

    // ModelViewPerspective
    float MVP[16];
    mult(cam->perspective, orientation, MVP);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
}

void objMV(GLint loc, Camera *cam, State *objState) {

    float translation[16];
    vec3 pos = objState->position;
    translate(translation, pos[0], pos[1], pos[2]);

    float orientation[16];
    rotate3D(orientation, objState->angles);

    float MV[16];
    mult(orientation, translation, MV);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MV);
}

void objMVnoTrans(GLint loc, Camera *cam, State *objState) {

    float MV[16];
    rotate3D(MV, objState->angles);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MV);
}