#include "GLContext.h"

using namespace std;

int main(int argc, char** argv) {
    
    GLContext* glc = new GLContext();

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

    glc->loadShader("vertex", GL_VERTEX_SHADER);
    glc->loadShader("fragment", GL_FRAGMENT_SHADER);
    glc->applyShaders();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(glc->main)) {

        /* --- rendering --- */
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glFlush();
        /* ---------------- */

        glfwSwapBuffers(glc->main);
        glfwPollEvents();
    }

    glc->~GLContext();
    return EXIT_SUCCESS;
}

