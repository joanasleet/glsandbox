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

void addShader(const char *srcFile, GLenum type, GLuint prog, Cache *shaderCache) {

    /* check for cached shaderId */
    GLuint shaderId = 0;
    lua_getfield( shaderCache, -1, srcFile );
    popInt( shaderCache, shaderId );

    if (!shaderId) {
        shaderId = compileShader(srcFile, glCreateShader(type));

        /* cache shaderId */
        lua_pushinteger( shaderCache, shaderId );
        lua_setfield( shaderCache, -2, srcFile );
    }

    /* add shader to gpu program */
    GLsizei logSize = 0;
    log_info("<Adding shader %s (id: %d)> to <program (id: %d)>", srcFile, shaderId, prog);
    glAttachShader(prog, shaderId);
    glLinkProgram(prog);

    /* check errors */
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logSize);
    char logMsg[logSize];
    glGetProgramInfoLog(prog, logSize, &logSize, logMsg);
    if (logSize > 0) log_err("%s", logMsg);
}

GLuint compileShader(const char *srcFile, GLuint shaderId) {

    char *shaderSrc = bufferFile(srcFile);

    log_info("<Compiling shader %s>", srcFile);

    glShaderSource(shaderId, 1, (const char **) &shaderSrc, NULL);
    glCompileShader(shaderId);
    free(shaderSrc);

    GLsizei logSize = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    char logMsg[logSize];
    glGetShaderInfoLog(shaderId, logSize, &logSize, logMsg);
    if (logSize > 0) log_err("%s", logMsg);
    return shaderId;
}

/* uniform setter */
void P(GLint loc, Camera *cam, State *objState, double globalTime) {

    glUniformMatrix4fv(loc, 1, GL_FALSE, cam->perspective);
}

void MV(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *state = cam->state;

    // orientation
    float rotaQ[] = { state->orientation[0], state->orientation[1], state->orientation[2], state->orientation[3] };
    quatInv(rotaQ);

    GLfloat orientation[16];
    quatToMat(orientation, rotaQ);

    // translation
    GLfloat translation[16];
    mat4trans(translation, -state->position[0], -state->position[1], -state->position[2]);

    // ModelView
    GLfloat MVmat[16];
    mat4mult(orientation, translation, MVmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVmat);
}

void MVnoTrans(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *state = cam->state;

    // orientation
    float rotaQ[4];
    rotate3D(rotaQ, state->angles);
    quatInv(rotaQ);

    GLfloat orientation[16];
    quatToMat(orientation, rotaQ);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, orientation);
}

void MVP(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *state = cam->state;

    // orientation
    float rotaQ[] = { state->orientation[0], state->orientation[1], state->orientation[2], state->orientation[3] };
    quatInv(rotaQ);

    GLfloat orientation[16];
    quatToMat(orientation, rotaQ);

    // translation
    GLfloat translation[16];
    mat4trans(translation, -state->position[0], -state->position[1], -state->position[2]);

    GLfloat temp[16];
    mat4mult(cam->perspective, orientation, temp);
    
    // (perspective * orientation) * translation
    GLfloat MVPmat[16];
    mat4mult(temp, translation, MVPmat);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVPmat);
}

void MVPnoTrans(GLint loc, Camera *cam, State *objState, double globalTime) {

    State *camState = cam->state;

    // orientation
    float rotaQ[] = { camState->orientation[0], camState->orientation[1], camState->orientation[2], camState->orientation[3] };
    //rotate3D(rotaQ, camState->angles);
    quatInv(rotaQ);

    GLfloat orientation[16];
    quatToMat(orientation, rotaQ);

    // ModelViewPerspective
    float MVP[16];
    mat4mult(cam->perspective, orientation, MVP);

    // update uniform
    glUniformMatrix4fv(loc, 1, GL_FALSE, MVP);
}

// buggy stuff here

void objMV(GLint loc, Camera *cam, State *objState, double globalTime) {

    float rotaQ[4];
    rotate3D(rotaQ, cam->state->angles);
    quatInv(rotaQ);

    GLfloat orientation[16];
    quatToMat(orientation, rotaQ);

    float translation[16];
    vec3 pos = objState->position;
    mat4trans(translation, pos[0], pos[1], pos[2]);

    float MV[16];
    mat4mult(orientation, translation, MV);

    glUniformMatrix4fv(loc, 1, GL_FALSE, MV);
}

void objMVnoTrans(GLint loc, Camera *cam, State *objState, double globalTime) {

    float rotaQ[4];
    rotate3D(rotaQ, cam->state->angles);
    quatInv(rotaQ);

    GLfloat orientation[16];
    quatToMat(orientation, rotaQ);

    glUniformMatrix4fv(loc, 1, GL_FALSE, orientation);
}

void camPos(GLint loc, Camera *cam, State *objState, double globalTime) {
    glUniform3fv(loc, 1, cam->state->position);
}

void gTime(GLint loc, Camera *cam, State *objState, double globalTime) {
    glUniform1f(loc, globalTime);
}

/* 
 * helper */

char *bufferFile(const char *path) {

    FILE *file = fopen(path, "rb");
    exit_guard(file);
    exit_guard(fseek(file, 0L, SEEK_END) == 0);
    long int size = ftell(file);
    fclose(file);

    file = fopen(path, "r");

    char *fileContent;

    if (file && (size > 0)) {
        fileContent = (char *) malloc(sizeof (char) * (size + 1));
        fread(fileContent, sizeof(char), size, file);
        fileContent[size] = '\0';
    } else {
        fclose(file);
        log_err("%s missing or empty.", path);
        exit(EXIT_SUCCESS);
    }

    fclose(file);
    return fileContent;
}

