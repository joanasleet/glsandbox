#include "Shader.h"
#include "MatrixMath.h"

Shader *newShader() {

    Shader *shader = NEW(Shader);
    shader->program = glCreateProgram();
    return shader;
}

void freeShader(Shader *shader) {

    glDeleteProgram(shader->program);
    // stages are freed by the cache
    free(shader->setters);
    // uniforms are freed by the cache

    free(shader);
}

/* uniform setter */
void P(GLint loc, Camera *cam, State *objState) {

    glUniformMatrix4fv(loc, 1, GL_FALSE, cam->perspective);
}

void MV(GLint loc, Camera *cam, State *objState) {

    // orientation
    GLfloat orientation[16];
    quatToMat(cam->state->orientation, orientation);

    // translation
    mat4 translation = cam->translation;

    // ModelView
    GLfloat MVmat[16];
    mult(orientation, translation, MVmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVmat);
}

void MVP(GLint loc, Camera *cam, State *objState) {

    // perspective
    mat4 P = cam->perspective;

    // orientation
    GLfloat orientation[16];
    quatToMat(cam->state->orientation, orientation);

    // translation
    mat4 translation = cam->translation;

    // perspective * orientation
    GLfloat VPmat[16];
    mult(P, orientation, VPmat);

    // (perspective * orientation) * translation
    GLfloat MVPmat[16];
    mult(VPmat, translation, MVPmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVPmat);
}

void MVPnoTrans(GLint loc, Camera *cam, State *objState) {

    mat4 P = cam->perspective;

    // orientation
    GLfloat orientation[16];
    quatToMat(cam->state->orientation, orientation);

    // ModelViewPerspective
    float MVP[16];
    mult(P, orientation, MVP);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
}

void objMV(GLint loc, Camera *cam, State *objState) {

    float translation[16];
    vec3 pos = objState->position;
    translate(translation, pos[0], pos[1], pos[2]);

    float orientation[16];
    rotateQ(orientation, objState->orientation);

    float MV[16];
    mult(orientation, translation, MV);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MV);
}

void objMVnoTrans(GLint loc, Camera *cam, State *objState) {

    float MV[16];
    rotateQ(MV, objState->orientation);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MV);
}