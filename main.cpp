#include "GLContext.h"
#include "GLProgram.h"
#include "GLHelper.h"

#include <iostream>

#define COLORS 100

#define RESX 1024
#define RESY 768

using namespace std;

int main(int argc, char** argv) {

    GLContext* glc = new GLContext();
    glPointSize(2);

    GLsizei bs = sizeof (GLdouble) * RESX * RESY * 2;
    GLdouble* area = createArea(-2, 1, -1, 1, RESX, RESY);

    GLuint vao = genId(VertexArray);
    glBindVertexArray(vao);

    GLuint buf = genId(Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, bs, area, GL_STATIC_DRAW);

    GLProgram* shaderProg = new GLProgram();
    shaderProg->loadShader("vertex", GL_VERTEX_SHADER);
    shaderProg->loadShader("fragment", GL_FRAGMENT_SHADER);
    shaderProg->use();

    GLfloat* cmap = colorMap(0.5, 0.5, 0.9, 1.0, 1.0, 1.0, COLORS);
    GLuint cmapVar = shaderProg->getVar("colors");
    glUniform1fv(cmapVar, 3 * COLORS, (const GLfloat*) cmap); // bug ?

    GLuint escapeVar = shaderProg->getVar("escape");
    glUniform1i(escapeVar, (const GLint) COLORS);

    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(glc->main) && !glfwGetKey(glc->main, GLFW_KEY_ESCAPE)) {

        /* --- rendering --- */
        glClear(GL_COLOR_BUFFER_BIT);

        // glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, RESX * RESY);

        glFlush();
        /* ---------------- */

        glfwSwapBuffers(glc->main);
        glfwPollEvents();
    }

    free(area);
    shaderProg->~GLProgram();
    glc->~GLContext();
    return EXIT_SUCCESS;
}

