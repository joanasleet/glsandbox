#include "Util.h"
#include "ShaderUtil.h"

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
