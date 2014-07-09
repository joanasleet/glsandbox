

#include "GLHelper.h"
#include "Camera.h"
#include "Context.h"
#include "Object.h"
#include "GLObject.h"

#include <iostream>

using namespace std;

/*************** SANDBOX ********************/



/* opengl context */
Context* context = createContext();

/* main shader program */
GLuint floorShader = glCreateProgram();
GLuint cubeShader = glCreateProgram();

/* main camera */
Camera* cam = createCamera(0, 10, 0);

bool mouseSet = false;

int main(int argc, char** argv) {

    config();

    /* ################# skybox ################## */
    /* ########################################### */

    Object* cubeTex = newObj(GL_TEXTURE_CUBE_MAP);
    glBindTexture(cubeTex->target, cubeTex->id);

    int w, h, c;

    unsigned char* cm_right = texBuffer("textures/sky1.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, cm_right);
    free(cm_right);

    unsigned char* cm_left = texBuffer("textures/sky4.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, cm_left);
    free(cm_left);

    unsigned char* cm_front = texBuffer("textures/sky5.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, cm_front);
    free(cm_front);

    unsigned char* cm_back = texBuffer("textures/sky2.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, cm_back);
    free(cm_back);

    unsigned char* cm_top = texBuffer("textures/sky3.bmp", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, cm_top);
    free(cm_top);

    unsigned char* cm_bot = texBuffer("textures/cmbot.png", &w, &h, &c);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, cm_bot);
    free(cm_bot);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    Object* cubeVao = cubeVAO(10000.0f, 10.0f, 0, 0, 0);

    /* ################# floor ################### */
    /* ########################################### */

    Object* planeVao = planeVAO(50000.0f, 50.0f, 0, 0, 0);

    Object* planeTex = texture("textures/concrete.jpg", GL_TEXTURE_2D);

    loadShader("floor.vs", GL_VERTEX_SHADER, floorShader);
    loadShader("floor.fs", GL_FRAGMENT_SHADER, floorShader);

    loadShader("cube.vs", GL_VERTEX_SHADER, cubeShader);
    loadShader("cube.fs", GL_FRAGMENT_SHADER, cubeShader);

    GLuint shaders[] = {floorShader, cubeShader};

    /* render loop */
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(cubeShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(cubeTex->target, cubeTex->id);
        glBindVertexArray(cubeVao->id);
        glDrawArrays(GL_QUADS, 0, 24);

        update(cam, context, shaders);

        glUseProgram(floorShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(planeTex->target, planeTex->id);
        glBindVertexArray(planeVao->id);
        glDrawArrays(GL_QUADS, 0, 4);

        update(cam, context, shaders);

        glFlush();
        glfwSwapBuffers(context->win);
        glfwPollEvents();
    }

    glDeleteProgram(floorShader);
    glDeleteProgram(cubeShader);
    free(context);
    free(cam);

    glfwTerminate();

    return EXIT_SUCCESS;
}

void config() {

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.7, 1.0);
    glfwSetInputMode(context->win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorPosCallback(context->win, cursorCB);
    glfwSetScrollCallback(context->win, scrollCB);
    glfwSetKeyCallback(context->win, keyCB);

}

void cursorCB(GLFWwindow* win, double xpos, double ypos) {

    double midX = context->xRes / 2.0f;
    double midY = context->yRes / 2.0f;

    cam->rotaX += midX - xpos;
    cam->rotaY += midY - ypos;

    glfwSetCursorPos(win, midX, midY);
}

void scrollCB(GLFWwindow* win, double xoffset, double yoffset) {

    cam->yPos += yoffset;
    cam->rotaZ += -xoffset;

    INFO("Cam position: (%.0f,%.0f,%.0f)", cam->xPos, cam->yPos, cam->zPos);
}

void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods) {

    switch (key) {
        case GLFW_KEY_W:

            if (action == GLFW_PRESS) {
                cam->zspeed = 5.0f;

            } else if (action == GLFW_RELEASE) {
                cam->zspeed = 0.0f;
            }
            break;
        case GLFW_KEY_A:

            if (action == GLFW_PRESS) {
                cam->xspeed = -5.0f;

            } else if (action == GLFW_RELEASE) {
                cam->xspeed = 0.0f;
            }
            break;
        case GLFW_KEY_S:

            if (action == GLFW_PRESS) {
                cam->zspeed = -5.0f;

            } else if (action == GLFW_RELEASE) {
                cam->zspeed = 0.0f;
            }
            break;
        case GLFW_KEY_D:

            if (action == GLFW_PRESS) {
                cam->xspeed = 5.0f;

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
