#include "GLContext.h"
#include "GLProgram.h"
#include "GLHelper.h"
#include "GLObject.h"

#include <iostream>

using namespace std;

/************* SANDBOX ********************/

/* opengl context */
GLContext* glc = new GLContext();

/* main shader program */
GLProgram* mainProg = new GLProgram();

// EXPERIMENTAL
double xpos, ypos;

#define ABS(x) ((x > 0) x : -x)

int main(int argc, char** argv) {

    setGLStates();

    /* -------------- cube -------------- */
    GLObject* vao_cube = new GLObject(GL_VERTEX_ARRAY);
    GLObject* vbo_cube = new GLObject(GL_ARRAY_BUFFER);
    static GLfloat cubeData[] = {
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
    vbo_cube->buffer(sizeof (cubeData), cubeData, GL_STATIC_DRAW);

    GLObject* ib_cube = new GLObject(GL_ELEMENT_ARRAY_BUFFER);
    GLuint cubeIndx[] = {
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
        3, 7, 6, 2,
    };
    ib_cube->buffer(sizeof (cubeIndx), cubeIndx);

    // texture
    GLObject* tex_cube = new GLObject(GL_TEXTURE_2D);
    tex_cube->loadTexture("tex0.png");
    tex_cube->param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    tex_cube->param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    tex_cube->param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex_cube->param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // set vertex buffer layout
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // texture offset
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*32));
    glEnableVertexAttribArray(1);

    /* ----------------- floor ------------------- */
    GLObject* vao_floor = new GLObject(GL_VERTEX_ARRAY);
    GLObject* vbo_floor = new GLObject(GL_ARRAY_BUFFER);
    static GLfloat floorData[] = {

        -100.0f, -1.0f, 100.0f, 1.0f, // 0
        100.0f, -1.0f, 100.0f, 1.0f, // 1
        100.0f, -1.0f, -100.0f, 1.0f, // 2
        -100.0f, -1.0f, -100.0f, 1.0f, // 3

        // tex coords
        0.0f, 0.0f,
        10.0f, 0.0f,
        10.0f, 10.0f,
        10.0f, 0.0f
    };
    vbo_floor->buffer(sizeof (floorData), floorData, GL_STATIC_DRAW);

    GLObject* ib_floor = new GLObject(GL_ELEMENT_ARRAY_BUFFER);
    GLuint floorIndx[] = {
        0, 1, 2, 3
    };
    ib_floor->buffer(sizeof (floorIndx), floorIndx);

    // texture
    GLObject* tex_floor = new GLObject(GL_TEXTURE_2D);
    tex_floor->loadTexture("floor.png");
    tex_floor->param(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex_floor->param(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex_floor->param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex_floor->param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // set vertex buffer layout
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // texture offset
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    glEnableVertexAttribArray(1);

    /* shader program setup */
    mainProg->loadShader("vertex", GL_VERTEX_SHADER);
    mainProg->loadShader("fragment", GL_FRAGMENT_SHADER);
    mainProg->use();

    /* MVP setup */

    glc->hideMouse();
    /* render loop */
    while (!glfwWindowShouldClose(glc->main) && !glfwGetKey(glc->main, GLFW_KEY_ESCAPE)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupMVP();

        glBindVertexArray(vao_cube->id);
        glBindTexture(tex_cube->target, tex_cube->id);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        
        
        glBindVertexArray(vao_floor->id);
        glBindTexture(tex_floor->target, tex_floor->id);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));


        glFlush();
        glfwSwapBuffers(glc->main);
        glfwPollEvents();
    }

    mainProg->~GLProgram();
    glc->~GLContext();

    return EXIT_SUCCESS;
}

void setupMVP() {

    // camera movement
    static double xpos_old, ypos_old;

    double midX, midY;
    midX = glc->resX / 2;
    midY = glc->resY / 2;

    double speed = 0.01;

    glfwGetCursorPos(glc->main, &xpos, &ypos);
    glfwSetCursorPos(glc->main, midX, midY);
    xpos_old += (midX - xpos);
    ypos_old += (midY - ypos);


    // Camera
    glm::mat4 yRotation = glm::rotate<float>(glm::mat4(), ypos_old * speed, glm::vec3(-1, 0, 0));
    glm::mat4 xyRotation = glm::rotate<float>(yRotation, xpos_old * speed, glm::vec3(0, 1, 0));

    // ModelView
    glm::mat4 modelView = glm::translate<float>(glm::mat4(), glm::vec3(0.0, 0.0, -15.0));

    // Perspective
    glm::mat4 perspective = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    // MVP
    glm::mat4 MVP = perspective * modelView*xyRotation;
    glUniformMatrix4fv(mainProg->getVar("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
}

void setGLStates() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2, 0.2, 1.0);
}