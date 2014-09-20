#include "ShaderUtil.h"
#include "Logger.h"

void addShader(const char* srcFile, GLenum type, GLuint prog, ShaderCache* shaderCache) {

    INFO("–");
    GLuint shaderId = get(shaderCache, srcFile);

    if (!shaderId) {
        shaderId = compileShader(srcFile, glCreateShader(type));
        cache(shaderCache, srcFile, shaderId);
    }

    GLsizei logSize = 0;

    INFO("Adding shader <%s>(ID: %d) to program %d", srcFile, shaderId, prog);
    glAttachShader(prog, shaderId);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logSize);
    char logMsg[logSize];
    glGetProgramInfoLog(prog, logSize, &logSize, logMsg);
    if (logSize > 0) {
        ERR("%s", logMsg);
    }
}

GLuint compileShader(const char* srcFile, GLuint shaderId) {

    char* shaderSrc = bufferFile(srcFile);

    INFO("Compiling shader <%s>", srcFile);
    //INFO("\n%s", shaderSrc);
    glShaderSource(shaderId, 1, (const char**) &shaderSrc, NULL);
    glCompileShader(shaderId);
    free(shaderSrc);

    GLsizei logSize = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    char logMsg[logSize];
    glGetShaderInfoLog(shaderId, logSize, &logSize, logMsg);
    if (logSize > 0) {
        ERR("%s", logMsg);
    }

    return shaderId;
}

char* bufferFile(const char* path) {

    char* fileContent = NULL;
    FILE* file = fopen(path, "r");

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

