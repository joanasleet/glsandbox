#include <cstdlib>
#include <stdio.h>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <string.h>

#define BUFFER_OFFSET(offset) ((void *) (offset))

using namespace std;

GLFWwindow* mainw;
GLuint mainProgramId;

char* bufferFile(const char* path) {

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

void loadShader(const char* srcFile, GLenum type) {

    GLuint shaderId = glCreateShader(type);

    /* get shader source */
    char* shaderSrc = bufferFile(srcFile);

    GLint resultFlag = GL_FALSE;
    int logSize;
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
    glGetShaderInfoLog(shaderId, logSize, NULL, logMsg);
    fprintf(stdout, "\tOK (Flag: %d)\n", resultFlag);
    if (strlen(logMsg) > 0) {
        fprintf(stdout, "Info:{%s}\n", logMsg);
    }

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
    glGetProgramInfoLog(mainProgramId, logSize, NULL, logMsg);
    fprintf(stdout, "\tOK (Flag: %d)\n", resultFlag);
    if (strlen(logMsg) > 0) {
        fprintf(stdout, "Info:{%s}\n", logMsg);
    }
    fprintf(stdout, "\n");

    glDeleteShader(shaderId);
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        fprintf(stderr, "ERROR starting GLFW.\n");
        exit(EXIT_FAILURE);
    }

    printf("GLFW initialized.\n");

    mainw = glfwCreateWindow(800, 600, "GL Sandbox", NULL, NULL);
    if (!mainw) {
        fprintf(stderr, "ERROR creating main window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(mainw);
    printf("Window created.\n");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR starting GLEW.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    printf("GLEW initialized.\n");

    GLuint vao[1];
    GLuint buffer[1];

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    GLfloat vertices[6][2] = {
        {-0.90, -0.90},
        { 0.85, -0.90},
        {-0.90, 0.85},
        { 0.90, -0.85},
        { 0.90, 0.90},
        {-0.85, 0.90}
    };

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

    loadShader("vertex", GL_VERTEX_SHADER);
    loadShader("fragment", GL_FRAGMENT_SHADER);
    glUseProgram(mainProgramId);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(mainw)) {

        /* --- rendering --- */
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glFlush();
        /* ---------------- */

        glfwSwapBuffers(mainw);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

