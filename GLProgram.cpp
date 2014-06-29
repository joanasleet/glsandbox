#include "GLProgram.h"

GLProgram::GLProgram() {
    printf("Loading program ");
    id = glCreateProgram();
    printf("(Id: %d). OK.\n", id);
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

    printf("Loading shader from file <%s>\n", srcFile);
    printf("––––––––––––––––––––––––––––––––––––––––––––––––––––––\n");
    printf("%s\n", shaderSrc);
    printf("—–————————————————————————————————————————————————————\n");

    /* compile shader */
    printf("Compiling...");
    glShaderSource(shaderId, 1, (const char**) &shaderSrc, NULL);
    glCompileShader(shaderId);
    printf("\tOK\n");
    free(shaderSrc);

    /* check shader */
    printf("Checking...");
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &resultFlag);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetShaderInfoLog(shaderId, logSize, &logSize, logMsg);
    if (logSize > 0) {
        printError((const char*) logMsg);
    } else {
        printf("\tOK\n");
    }
    logSize = 0;
    free(logMsg);

    printf("Linking...");
    glAttachShader(id, shaderId);
    glLinkProgram(id);
    printf("\tOK\n");

    /* check main program */
    printf("Checking...");
    glGetProgramiv(id, GL_LINK_STATUS, &resultFlag);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logSize);
    logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetProgramInfoLog(id, logSize, &logSize, logMsg);
    if (logSize > 0) {
        printError((const char*) logMsg);
    } else {
        printf("\tOK\n");
    }
    printf("\n");
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
        fprintf(stderr, "Couldn't access file %s\n", path);
        exit(EXIT_FAILURE);
    }

    fseeko(file, 0L, SEEK_END);
    long int size = ftello(file);
    fclose(file);

    file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "Couldn't access file <%s>\n", path);
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
        fprintf(stderr, "File <%s> is empty. Aborting.\n", path);
        exit(EXIT_SUCCESS);
    }

    fclose(file);
    return fileContent;
}
