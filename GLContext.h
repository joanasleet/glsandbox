#ifndef GLCONTEXT_H
#define	GLCONTEXT_H

#include <cstdlib>
#include <stdio.h>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <string.h>

#define BUFFER_OFFSET(offset) ((void *) (offset))

class GLContext {
public:
    GLFWwindow* main;
    GLuint mainProgramId;

    GLContext(int resX = 800, int resY = 600, const char* title = "OpenGL");
    ~GLContext();

    void loadShader(const char* source, GLenum type);
    void applyShaders();

    void setRes(int x, int y);
    void printSpec();

    void toString();

private:
    int resX;
    int resY;

    char* bufferFile(const char* path);
};

GLContext::GLContext(int resX, int resY, const char* title) {
    setRes(resX, resY);
    mainProgramId = 0;

    if (!glfwInit()) {
        fprintf(stderr, "ERROR starting GLFW.\n");
        exit(EXIT_FAILURE);
    }

    printf("GLFW initialized.\n");

    main = glfwCreateWindow(resX, resY, title, NULL, NULL);
    if (!main) {
        fprintf(stderr, "ERROR creating main window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(main);
    printf("Window created.\n");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR starting GLEW.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    printf("GLEW initialized.\n");
}

GLContext::~GLContext() {
    glfwTerminate();
}

void GLContext::loadShader(const char* srcFile, GLenum type) {

    GLuint shaderId = glCreateShader(type);

    /* get shader source */
    char* shaderSrc = bufferFile(srcFile);

    GLint resultFlag = GL_FALSE;
    int logSize;
    //char* logMsg;
    char logMsg[200];

    fprintf(stdout, "##################\n");
    fprintf(stdout, "# Loading shader # (%s)\n", srcFile);
    fprintf(stdout, "##################\n");

    fprintf(stdout, "____________________ Shader source _____________________\n");
    fprintf(stdout, "%s\n", shaderSrc);
    fprintf(stdout, "————————————————————————————————————————————————————————\n");

    /* compile shader */
    fprintf(stdout, "Compiling...");
    glShaderSource(shaderId, 1, (const char**) &shaderSrc, NULL);
    glCompileShader(shaderId);
    fprintf(stdout, "\tOK\n");
    free(shaderSrc);

    /* check shader */
    fprintf(stdout, "Checking...");
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &resultFlag);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);
    //logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetShaderInfoLog(shaderId, logSize, NULL, logMsg);
    fprintf(stdout, "\tOK (Flag: %d)\n", resultFlag);
    if (strlen(logMsg) > 0) {
        fprintf(stdout, "Info:{%s}\n", logMsg);
    }
    //free(logMsg);


    /* link main program */
    fprintf(stdout, "\n########################\n");
    fprintf(stdout, "# Loading main program # ");
    if (mainProgramId == 0) {
        mainProgramId = glCreateProgram();
    }
    fprintf(stdout, "(ID: %d)", mainProgramId);
    fprintf(stdout, "\n########################\n");

    fprintf(stdout, "Linking...");
    glAttachShader(mainProgramId, shaderId);
    glLinkProgram(mainProgramId);
    fprintf(stdout, "\tOK\n");

    /* check main program */
    fprintf(stdout, "Checking...");
    glGetProgramiv(mainProgramId, GL_LINK_STATUS, &resultFlag);
    glGetProgramiv(mainProgramId, GL_INFO_LOG_LENGTH, &logSize);
    //logMsg = (char*) malloc(sizeof (char) * logSize);
    glGetProgramInfoLog(mainProgramId, logSize, NULL, logMsg);
    fprintf(stdout, "\tOK (Flag: %d)\n", resultFlag);
    if (strlen(logMsg) > 0) {
        fprintf(stdout, "Info:{%s}\n", logMsg);
    }
    fprintf(stdout, "\n");
    //free(logMsg);

    glDeleteShader(shaderId);
}

void GLContext::applyShaders() {
    glUseProgram(mainProgramId);
}

char* GLContext::bufferFile(const char* path) {

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

void GLContext::setRes(int x, int y) {
    resX = x;
    resY = y;
}

void GLContext::toString() {
    printf("GLContext:\n\tresX: %d\n\tresY: %d\n", resX, resY);
}

#endif	/* GLCONTEXT_H */

