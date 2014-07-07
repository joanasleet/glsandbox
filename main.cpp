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
Camera* cam = createCamera(0, 5, 0);

int main(int argc, char** argv) {

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glfwSetInputMode(context->win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorPos(context->win, context->xRes / 2, context->yRes / 2);

    glfwSetScrollCallback(context->win, scrollCB);
    glfwSetKeyCallback(context->win, keyCB);



    // textures
    GLObject* tex_cube = new GLObject(GL_TEXTURE_2D);
    tex_cube->loadTexture("tex0.png");

    GLObject* tex_floor = new GLObject(GL_TEXTURE_2D);
    tex_floor->loadTexture("plane.png");

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
    cam->yPos += yoffset;
    INFO("(x,y) offset: (%.1d,%.1d)", xoffset, yoffset);

}

void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods) {

    float speed = 3.0f;

    switch (key) {
        case GLFW_KEY_W:
            cam->zPos += speed;
            break;
        case GLFW_KEY_A:
            cam->xPos += speed;
            break;
        case GLFW_KEY_S:
            cam->zPos += -speed;
            break;
        case GLFW_KEY_D:
            cam->xPos += -speed;
            break;
        default:

            break;
    }
    
    INFO("Cam position: (%.0f,%.0f,%.0f)", cam->xPos, cam->yPos, cam->zPos);
}
