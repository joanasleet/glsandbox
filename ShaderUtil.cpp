#include "ShaderUtil.h"

GLenum SHADER_TYPE[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER
};

extern ShaderCache* shaderCache;

void addShader(const char* srcFile, GLenum type, GLuint prog) {

    INFO("–");
    GLuint shaderId = get(shaderCache, srcFile);

    if (!shaderId) {
        shaderId = compileShader(srcFile, type);
        cache(shaderCache, srcFile, shaderId);
    }

    char* logMsg;
    GLsizei logSize = 0;

    INFO("Adding shader <%s>(ID: %d) to program %d", srcFile, shaderId, prog);
    glAttachShader(prog, shaderId);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logSize);
    logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetProgramInfoLog(prog, logSize, &logSize, logMsg);
    if (logSize > 0) {
        ERR("%s", logMsg);
    }

    free(logMsg);
}

GLuint compileShader(const char* srcFile, GLenum type) {

    GLuint shaderId = glCreateShader(type);

    char* shaderSrc = bufferFile(srcFile);

    char* logMsg;
    GLsizei logSize = 0;


    INFO("Compiling shader <%s>", srcFile);
    //INFO("\n%s", shaderSrc);
    glShaderSource(shaderId, 1, (const char**) &shaderSrc, NULL);
    glCompileShader(shaderId);
    free(shaderSrc);

    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetShaderInfoLog(shaderId, logSize, &logSize, logMsg);
    if (logSize > 0) {
        ERR("%s", logMsg);
    }
    logSize = 0;
    free(logMsg);

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

