#include "ShaderUtil.h"
#include "Debugger.h"
#include "Deallocator.h"

#include <stdio.h>

void addShader(const char *srcFile, GLenum type, GLuint prog, Cache *shaderCache) {

    info("%s", "---");
    GLuint shaderId = get(shaderCache, srcFile);

    if (!shaderId) {
        shaderId = compileShader(srcFile, glCreateShader(type));
        cache(shaderCache, srcFile, shaderId);
        storeShader(shaderId);
    }

    GLsizei logSize = 0;

    info("Adding shader <%s>(ID: %d) to program %d", srcFile, shaderId, prog);
    glAttachShader(prog, shaderId);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logSize);
    char logMsg[logSize];
    glGetProgramInfoLog(prog, logSize, &logSize, logMsg);
    if (logSize > 0) {
        err("%s", logMsg);
    }
}

GLuint compileShader(const char *srcFile, GLuint shaderId) {

    char *shaderSrc = bufferFile(srcFile);

    info("Compiling shader <%s>", srcFile);

    glShaderSource(shaderId, 1, (const char **) &shaderSrc, NULL);
    glCompileShader(shaderId);
    free(shaderSrc);

    GLsizei logSize = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    char logMsg[logSize];
    glGetShaderInfoLog(shaderId, logSize, &logSize, logMsg);
    if (logSize > 0) {
        err("%s", logMsg);
    }

    return shaderId;
}

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
        err("File <%s> missing or empty.", path);
        exit(EXIT_SUCCESS);
    }

    fclose(file);
    return fileContent;
}