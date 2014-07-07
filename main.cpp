#include "GLHelper.h"
#include "GLObject.h"
#include "Camera.h"
#include "Context.h"

#include <iostream>

using namespace std;

/*************** SANDBOX ********************/

/* opengl context */
Context* context = createContext();

/* main shader program */
GLuint shaderProg = glCreateProgram();

/* main camera */
Camera* cam = createCamera(0, 15, 0);

int main(int argc, char** argv) {

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.7, 1.0);
    glfwSetInputMode(context->win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorPos(context->win, context->xRes / 2, context->yRes / 2);

    glfwSetScrollCallback(context->win, scrollCB);
    glfwSetKeyCallback(context->win, keyCB);

    // textures
    GLObject* tex_cube = new GLObject(GL_TEXTURE_2D);
    tex_cube->loadTexture("textures/tex0.png");

    GLObject* tex_floor = new GLObject(GL_TEXTURE_2D);
    tex_floor->loadTexture("textures/plane.png");

    /* ################# floor ################### */
    /* ########################################### */

    /* VAO */
    GLObject* vao_floor = new GLObject(GL_VERTEX_ARRAY);
    GLObject* vbo_floor = new GLObject(GL_ARRAY_BUFFER);

    /* VBO */
    GLsizei vbo_size;
    GLfloat* floorData = planeVD(&vbo_size, 10000.0f);
    vbo_floor->buffer(vbo_size, floorData, GL_STATIC_DRAW);

    /* EBO */
    GLObject* ib_floor = new GLObject(GL_ELEMENT_ARRAY_BUFFER);
    GLuint floorIndx[] = {
        0, 1, 2, 3
    };
    ib_floor->buffer(sizeof (floorIndx), floorIndx);

    // VBO layout
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // texture offset
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    glEnableVertexAttribArray(1);

    /* shader program setup */
    loadShader("vertex", GL_VERTEX_SHADER, shaderProg);
    loadShader("fragment", GL_FRAGMENT_SHADER, shaderProg);
    glUseProgram(shaderProg);

    glBindVertexArray(vao_floor->id);
    glBindTexture(tex_floor->target, tex_floor->id);

    /* render loop */
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update(cam, context, shaderProg);

        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        glFlush();
        glfwSwapBuffers(context->win);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProg);
    free(context);
    free(cam);

    glfwTerminate();

    return EXIT_SUCCESS;
}

void scrollCB(GLFWwindow* win, double xoffset, double yoffset) {
    INFO("Scroll input (%.0f, %.0f)", xoffset, yoffset);

    cam->yPos += yoffset;
    cam->rotaZ += -xoffset;
}

void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods) {

    switch (key) {
        case GLFW_KEY_W:

            if (action == GLFW_PRESS) {
                cam->zspeed = 3.0f;
            } else if (action == GLFW_RELEASE) {
                cam->zspeed = 0.0f;
            }
            break;
        case GLFW_KEY_A:

            if (action == GLFW_PRESS) {
                cam->xspeed = -3.0f;
            } else if (action == GLFW_RELEASE) {
                cam->xspeed = 0.0f;
            }
            break;
        case GLFW_KEY_S:

            if (action == GLFW_PRESS) {
                cam->zspeed = -3.0f;
            } else if (action == GLFW_RELEASE) {
                cam->zspeed = 0.0f;
            }
            break;
        case GLFW_KEY_D:

            if (action == GLFW_PRESS) {
                cam->xspeed = 3.0f;
            } else if (action == GLFW_RELEASE) {
                cam->xspeed = 0.0f;
            }
            break;
        default:
            return;
            break;
    }

    INFO("Cam position: (%.0f,%.0f,%.0f)", cam->xPos, cam->yPos, cam->zPos);
}
