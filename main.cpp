#include "GLHelper.h"
#include "Camera.h"
#include "Context.h"
#include "Object.h"

#include <iostream>

using namespace std;

/* opengl context */
Context* context = createContext();

/* shaders */
GLuint floorShader = glCreateProgram();
GLuint cubeShader = glCreateProgram();

GLint mvpLoc[2];

/* main camera */
#define CAM_YOFFSET (-1580)
Camera* cam = createCamera(0, CAM_YOFFSET, 0);

bool mouseGrabbed = false;

int main(int argc, char** argv) {

    config();

    /* ############# skybox ############### */
    Object* cubeVao = cubeVAO(100000.0f, 10.0f);

    const char* cubeFaces[] = {
        "textures/sky1.bmp",
        "textures/sky4.bmp",
        "textures/sky3.bmp",
        "textures/cmbot.png",
        "textures/sky5.bmp",
        "textures/sky2.bmp"
    };
    Object* cubeTex = cubeMap(cubeFaces);

    loadShader("cube.vs", GL_VERTEX_SHADER, cubeShader);
    loadShader("cube.fs", GL_FRAGMENT_SHADER, cubeShader);

    /* ############# floor ################# */
    Object* planeVao = planeVAO(50000.0f, 50.0f, 0, CAM_YOFFSET - 20, 0);

    Object* planeTex = texture("textures/concrete.jpg", GL_TEXTURE_2D);

    loadShader("floor.vs", GL_VERTEX_SHADER, floorShader);
    loadShader("floor.fs", GL_FRAGMENT_SHADER, floorShader);
    
    mvpLoc[0] = glGetUniformLocation(floorShader, "MVP");
    mvpLoc[1] = glGetUniformLocation(cubeShader, "MVP");
    
    
    /* render loop */
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        fps();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update(cam);
        
        updateMVP(floorShader, mvpLoc[0]);
        //glUseProgram(floorShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(planeTex->target, planeTex->id);
        glBindVertexArray(planeVao->id);
        glDrawArrays(GL_QUADS, 0, 4);

        updateMVP(cubeShader, mvpLoc[1]);
        glUseProgram(cubeShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(cubeTex->target, cubeTex->id);
        glBindVertexArray(cubeVao->id);
        glDrawArrays(GL_QUADS, 0, 24);


        glFlush();
        glfwSwapBuffers(context->win);
        glfwPollEvents();
    }

    glDeleteProgram(floorShader);
    glDeleteProgram(cubeShader);

    free(cubeVao);
    free(cubeTex);

    free(planeVao);
    free(planeTex);

    free(context);
    free(cam);

    glfwTerminate();

    return EXIT_SUCCESS;
}

void config() {

    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glClearColor(0.5, 0.5, 0.7, 1.0);
    glfwSetInputMode(context->win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetCursorEnterCallback(context->win, cursorEnterCB);
    glfwSetScrollCallback(context->win, scrollCB);
    glfwSetKeyCallback(context->win, keyCB);
}

void cursorCB(GLFWwindow* win, double xpos, double ypos) {

    cam->rotaX += (context->xRes / 2.0f) - xpos;
    cam->rotaY += (context->yRes / 2.0f) - ypos;

    glfwSetCursorPos(win, (context->xRes / 2.0f), (context->yRes / 2.0f));

    INFO("Cam direction: (%.3f, %.3f, %.3f)", cam->dirX, cam->dirY, cam->dirZ);
}

void cursorEnterCB(GLFWwindow* win, int enter) {
    if (enter == GL_TRUE) {

    }
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
                cam->zspeed = 3.0f;
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
        case GLFW_KEY_C:

            if (action == GLFW_PRESS && !mouseGrabbed) {
                glfwSetCursorPos(win, context->xRes / 2.0f, context->yRes / 2.0f);
                glfwSetCursorPosCallback(win, cursorCB);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                mouseGrabbed = true;
            } else if (action == GLFW_PRESS && mouseGrabbed) {
                glfwSetCursorPosCallback(win, NULL);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                mouseGrabbed = false;
            }
            break;
        default:
            return;
            break;
    }

    INFO("Cam position: (%.0f,%.0f,%.0f)", cam->xPos, cam->yPos, cam->zPos);
}

void fps() {
    static double prevTime = glfwGetTime();
    double currTime = glfwGetTime();
    double diffTime = currTime - prevTime;
    static int frames = 0;

    if (diffTime > 0.5) {
        prevTime = currTime;
        char title[20];
        sprintf(title, "OpenGL @ %.2f", (double) frames / diffTime);
        glfwSetWindowTitle(context->win, title);
        frames = 0;
    }
    ++frames;
}

void updateMVP(GLuint prog, GLint location) {

    glm::mat4 translateCamera = glm::translate<float>(glm::mat4(1.0f), glm::vec3(-cam->xPos, -cam->yPos, -cam->zPos));
    glm::mat4 yRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaY * CAM_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaX * CAM_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaZ * CAM_SPEED, glm::vec3(0, 0, 1));
    glm::mat4 perspective = glm::infinitePerspective(FOV, ASPECT_RATIO, NEAR_PLANE);

    glm::mat4 MVP = perspective * yRota * xRota * zRota * translateCamera;

    glUseProgram(prog);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP));
}