#include "GLContext.h"
#include "GLProgram.h"
#include "GLHelper.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {

    GLContext* glc = new GLContext();
    glPointSize(1.0);

    int size = 1200;
    GLsizei bs = sizeof (GLfloat) * size * size * 2;
    GLfloat* area = createArea(-2, 1, -1, 1, size);

    GLuint vao = genId(VertexArray);
    glBindVertexArray(vao);

    GLuint buf = genId(Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, bs, area, GL_STATIC_DRAW);

    GLProgram* shaderProg = new GLProgram();
    shaderProg->loadShader("vertex", GL_VERTEX_SHADER);
    shaderProg->loadShader("fragment", GL_FRAGMENT_SHADER);
    shaderProg->use();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    //printArea(area, size);
    while (!glfwWindowShouldClose(glc->main)) {

        /* --- rendering --- */
        glClear(GL_COLOR_BUFFER_BIT);

        // glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, size * size);

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

