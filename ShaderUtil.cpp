#include "ShaderUtil.h"
#include "Debugger.h"

void addShader(const char* srcFile, GLenum type, GLuint prog, ShaderCache* shaderCache) {

    info("–");
    GLuint shaderId = get(shaderCache, srcFile);

    if (!shaderId) {
        shaderId = compileShader(srcFile, glCreateShader(type));
        cache(shaderCache, srcFile, shaderId);
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

GLuint compileShader(const char* srcFile, GLuint shaderId) {

    char* shaderSrc = bufferFile(srcFile);

    info("Compiling shader <%s>", srcFile);
    
    glShaderSource(shaderId, 1, (const char**) &shaderSrc, NULL);
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

char* bufferFile(const char* path) {

    char* fileContent = NULL;
    FILE* file = fopen(path, "r");

    if (!file) {
        err("Couldn't access file %s", path);
        exit(EXIT_FAILURE);
    }

    fseeko(file, 0L, SEEK_END);
    long int size = ftello(file);
    fclose(file);

    file = fopen(path, "r");
    if (!file) {
        err("Couldn't access file <%s>", path);
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
        err("File <%s> is empty. Aborting.", path);
        exit(EXIT_SUCCESS);
    }

    fclose(file);
    return fileContent;
}

