#include "GLContext.h"
#include "GLProgram.h"
#include "GLHelper.h"
#include "GLObject.h"

#include <iostream>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void setGLStates();
void setupMVP();

/************* SANDBOX ********************/

/* opengl context */
GLContext* glc = new GLContext();

/* main shader program */
GLProgram* mainProg = new GLProgram();

int main(int argc, char** argv) {

    setGLStates();

    GLObject* vao = new GLObject(GL_VERTEX_ARRAY);

    GLObject* vertexBuffer = new GLObject(GL_ARRAY_BUFFER);

    static GLfloat coords[] = {
        // front vertices
        -1.0f, 1.0f, 1.0f, 1.0f, // 0
        -1.0f, -1.0f, 1.0f, 1.0f, // 1
        1.0f, -1.0f, 1.0f, 1.0f, // 2
        1.0f, 1.0f, 1.0f, 1.0f, // 3

        // back vertices
        -1.0f, 1.0f, -1.0f, 1.0f, // 4
        -1.0f, -1.0f, -1.0f, 1.0f, // 5
        1.0f, -1.0f, -1.0f, 1.0f, // 6
        1.0f, 1.0f, -1.0f, 1.0f, // 7

        // texture coords
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    /*
    static GLfloat colors[] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };
     */

    vertexBuffer->buffer(sizeof (coords), coords, GL_STATIC_DRAW);
    /*
    vertexBuffer->subBuffer(0, sizeof (coords), coords);
    vertexBuffer->subBuffer(sizeof (coords), sizeof (colors), colors);
     */

    GLObject* indexBuffer = new GLObject(GL_ELEMENT_ARRAY_BUFFER);
    GLuint indices[] = {
        // front
        0, 1, 2, 3,
        // back
        4, 5, 6, 7,

        // top
        0, 3, 7, 4,
        // bot
        1, 5, 6, 2,

        // left
        0, 4, 5, 1,
        // right
        3, 7, 6, 2
    };
    indexBuffer->buffer(sizeof (indices), indices, GL_STATIC_DRAW);

    int x, y, n;
    int channels = 4;
    const char* texFile = "textureBump.png";
    unsigned char* image_data = stbi_load(texFile, &x, &y, &n, channels);
    if (!image_data) {
        char str[20];
        sprintf(str, "Failed to load texture from file <%s>", texFile);
        printError(str);
    }
    fprintf(stdout, "Texture attribute: %i x %i (%i)\n", x, y, n);

    GLObject* tex = new GLObject(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    glTexImage2D(tex->target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* shader program setup */
    mainProg->loadShader("vertex", GL_VERTEX_SHADER);
    mainProg->loadShader("fragment", GL_FRAGMENT_SHADER);
    mainProg->use();

    /* MVP setup */
    setupMVP();

    // set vertex buffer layout
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*32));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /* render loop */
    while (!glfwWindowShouldClose(glc->main) && !glfwGetKey(glc->main, GLFW_KEY_ESCAPE)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao->id);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

        glFlush();
        glfwSwapBuffers(glc->main);
        glfwPollEvents();
    }

    mainProg->~GLProgram();
    glc->~GLContext();

    return EXIT_SUCCESS;
}

void setupMVP() {

    glm::mat4 view = glm::rotate<float>(glm::mat4(), 10.0, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(mainProg->getVar("V"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 model = glm::translate<float>(glm::mat4(), glm::vec3(0.0, 0.0, -3.0));
    glUniformMatrix4fv(mainProg->getVar("M"), 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 perspective = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(mainProg->getVar("P"), 1, GL_FALSE, glm::value_ptr(perspective));
}

void setGLStates() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.35, 0.7, 0.8, 1.0);
}