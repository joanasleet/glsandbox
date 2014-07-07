#include "GLProgram.h"

GLProgram::GLProgram() {

    id = glCreateProgram();
    INFO("Program loaded (Id: %d).", id);
}

GLProgram::~GLProgram() {
    glDeleteProgram(id);
}

void GLProgram::loadShader(const char* srcFile, GLenum type) {

    GLuint shaderId = glCreateShader(type);

    /* get shader source */
    char* shaderSrc = bufferFile(srcFile);

    GLint resultFlag = GL_FALSE;
    GLsizei logSize = 0;
    char* logMsg;

    INFO("Loading shader from file <%s>", srcFile);
    INFO("\n%s", shaderSrc);
    
    /* compile shader */
    glShaderSource(shaderId, 1, (const char**) &shaderSrc, NULL);
    glCompileShader(shaderId);
    free(shaderSrc);

    /* check shader */
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &resultFlag);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetShaderInfoLog(shaderId, logSize, &logSize, logMsg);
    if (logSize > 0) {
        ERR("%s", logMsg);
    } else {
        INFO("Shader compiled.");
    }
    logSize = 0;
    free(logMsg);

    glAttachShader(id, shaderId);
    glLinkProgram(id);
    INFO("Shader program linked.");

    /* check main program */
    glGetProgramiv(id, GL_LINK_STATUS, &resultFlag);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);
    logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetProgramInfoLog(id, logSize, &logSize, logMsg);
    if (logSize > 0) {
        ERR("%s", logMsg);
    } else {
        INFO("\t-> No errors.");
    }
    free(logMsg);

    glDeleteShader(shaderId);
}

GLuint GLProgram::getVar(const char* varName) {
    return glGetUniformLocation(id, varName);
}

void GLProgram::use() {
    glUseProgram(id);
}

char* GLProgram::bufferFile(const char* path) {

    char * fileContent = NULL;
    FILE * file = fopen(path, "r");

    if (!file) {
        ERR("Couldn't access file %s", path);
        exit(EXIT_FAILURE);
    }

    fseeko(file, 0L, SEEK_END);
    long int size = ftello(file);
    fclose(file);

    file = fopen(path, "r");

    if (!file) {
        ERR("Couldn't access file <%s>", path);
        exit(EXIT_FAILURE);
    }

    if (size) {
        fileContent = (char*) malloc(sizeof (char) * (size + 1));
        int i = 0;
        int c;
        while ((c = getc(file)) != EOF) {
            fileContent[i++] = c;
        }
        fileContent[i] = '\0';
    } else {
        fclose(file);
        ERR("File <%s> is empty. Aborting.", path);
        exit(EXIT_SUCCESS);
    }

    fclose(file);
    return fileContent;
}
