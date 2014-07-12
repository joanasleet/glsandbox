#include "GLHelper.h"
#include "Camera.h"
#include "Context.h"
#include "Object.h"
#include "Logger.h"

#include <iostream>

using namespace std;

/* opengl context */
Context* context = createContext();

/* shaders */
GLuint floorShader = glCreateProgram();
GLuint cubeShader = glCreateProgram();
GLuint lcubeShader = glCreateProgram();

GLint mvpLoc[10];

/* main camera */
#define CAM_YOFFSET (-1580)
Camera* cam = createCamera(0, CAM_YOFFSET, 5000);

int main(int argc, char** argv) {

    config();

    /* ############# skybox ############### */
    Object* cubeVao = cubeMapVAO(50000.0f, 10.0f);

    const char* cubeFaces[] = {
        "textures/SBRIGHT.png",
        "textures/SBLEFT.png",
        "textures/SBTOP.png",
        "textures/SBBOTTOM.png",
        "textures/SBFRONT.png",
        "textures/SBBACK.png"
    };
    Object* cubeTex = cubeMapTex(cubeFaces);

    loadShader("cube.vs", GL_VERTEX_SHADER, cubeShader);
    loadShader("cube.fs", GL_FRAGMENT_SHADER, cubeShader);

    /* ############# floor ################# */
    Object* planeVao = planeVAO(10000.0f, 50.0f, 0, CAM_YOFFSET - 20, 0);

    Object* planeTex = texture("textures/concrete.jpg", GL_TEXTURE_2D);

    loadShader("floor.vs", GL_VERTEX_SHADER, floorShader);
    loadShader("floor.fs", GL_FRAGMENT_SHADER, floorShader);

    /* ############# little cube ############ */
    //Object* lcubeVao = cubeVAO(100.0f, 1.0f, 0.0f, CAM_YOFFSET-20+50, -250.0f);
    Object* lcubeVao = cubeVAO(3200.0f, 1.0f, 0.0f, 0.0f, 0.0f);

    glBindVertexArray(lcubeVao->id);
    Object* cubeNormals = newObj(GL_ARRAY_BUFFER);
    glBindBuffer(cubeNormals->target, cubeNormals->id);
    const GLfloat normals[] = {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };
    glBufferData(cubeNormals->target, sizeof (normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    loadShader("lcube.vs", GL_VERTEX_SHADER, lcubeShader);
    loadShader("lcube.fs", GL_FRAGMENT_SHADER, lcubeShader);

    const char* lcubeFaces[] = {
        "textures/brick.png"
    };
    Object* lcubeTex = cubeMapTex(lcubeFaces, true);

    mvpLoc[0] = glGetUniformLocation(floorShader, "MVP");
    mvpLoc[1] = glGetUniformLocation(cubeShader, "MVP");

    mvpLoc[2] = glGetUniformLocation(lcubeShader, "MV");
    mvpLoc[3] = glGetUniformLocation(lcubeShader, "P");


    /* render loop */
    while (!glfwWindowShouldClose(context->win) && !glfwGetKey(context->win, GLFW_KEY_ESCAPE)) {

        fps();
        printWatchLog();
        update(cam);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateMVP(floorShader, mvpLoc[0]);
        glUseProgram(floorShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(planeTex->target, planeTex->id);
        glBindVertexArray(planeVao->id);
        glDrawArrays(GL_QUADS, 0, 4);

        updateMVP(cubeShader, mvpLoc[1]);
        glUseProgram(cubeShader);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(cubeTex->target, cubeTex->id);
        glBindVertexArray(cubeVao->id);
        glDrawArrays(GL_QUADS, 0, 24);

        updateMVP(lcubeShader, mvpLoc[2]);
        glUseProgram(lcubeShader);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(lcubeTex->target, lcubeTex->id);
        glBindVertexArray(lcubeVao->id);
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

void cursorCB(GLFWwindow* win, double xpos, double ypos) {

    cam->rotaX += (context->xRes / 2.0f) - xpos;
    cam->rotaY += (context->yRes / 2.0f) - ypos;

    glfwSetCursorPos(win, (context->xRes / 2.0f), (context->yRes / 2.0f));

    printWatchLog();
}

void cursorEnterCB(GLFWwindow* win, int enter) {
    if (enter == GL_TRUE) {

    }
}

void scrollCB(GLFWwindow* win, double xoffset, double yoffset) {

    cam->defaultSpeed += 5 * yoffset;
    cam->defaultSpeed = MAX(cam->defaultSpeed, 0);

    //cam->zspeed = cam->defaultSpeed;
}

void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods) {

    switch (key) {
        case GLFW_KEY_W:

            if (action == GLFW_PRESS) {
                cam->zspeed = cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->zspeed = 0.0f;
            }
            break;
        case GLFW_KEY_A:

            if (action == GLFW_PRESS) {
                cam->xspeed = -cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->xspeed = 0.0f;
            }
            break;
        case GLFW_KEY_S:

            if (action == GLFW_PRESS) {
                cam->zspeed = -cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->zspeed = 0.0f;
            }
            break;
        case GLFW_KEY_D:

            if (action == GLFW_PRESS) {
                cam->xspeed = cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->xspeed = 0.0f;
            }
            break;
        case GLFW_KEY_SPACE:

            if (action == GLFW_PRESS) {
                cam->yspeed = cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->yspeed = 0.0f;
            }
            break;
        case GLFW_KEY_C:

            if (action == GLFW_PRESS && !cam->mouseGrab) {
                glfwSetCursorPos(win, context->xRes / 2.0f, context->yRes / 2.0f);
                glfwSetCursorPosCallback(win, cursorCB);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cam->mouseGrab = true;
            } else if (action == GLFW_PRESS && cam->mouseGrab) {
                glfwSetCursorPosCallback(win, NULL);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cam->mouseGrab = false;
            }
            break;
        default:
            return;
            break;
    }

    printWatchLog();
}

void resizeCB(GLFWwindow* win, int w, int h) {
    context->xRes = w;
    context->yRes = h;
    glViewport(0,0,w,h);
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
    glm::mat4 yRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaY * TURN_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaX * TURN_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaZ * TURN_SPEED, glm::vec3(0, 0, 1));
    glm::mat4 perspective = glm::infinitePerspective(FOV, ASPECT_RATIO, NEAR_PLANE);

    glm::mat4 MV = yRota * xRota * zRota * translateCamera;

    glm::mat4 MVP = perspective * MV;

    glUseProgram(prog);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(MVP));

    if (prog == 3) {
        glUniformMatrix4fv(glGetUniformLocation(prog, "MV"), 1, GL_FALSE, glm::value_ptr(MV));
        glUniformMatrix4fv(glGetUniformLocation(prog, "P"), 1, GL_FALSE, glm::value_ptr(perspective));
    }
}

void printWatchLog() {
    watchLog = fopen(WATCH_LOG_NAME, "a+");
    if (!watchLog) {
        ERR("Failed to open log <%s>", WATCH_LOG_NAME);
        return;
    }

    fprintf(watchLog, "\n–––––––––––––– Watch Log ––––––––––––––\n");
    fprintf(watchLog, "DIRECTION:\n\t%.1f  %.1f  %.1f\n", cam->dirX, cam->dirY, cam->dirZ);
    fprintf(watchLog, "POSITION:\n\t%.1f  %.1f  %.1f\n", cam->xPos, cam->yPos, cam->zPos);
    fprintf(watchLog, "SPEED:\n\t%.1f\n", cam->defaultSpeed);

    fclose(watchLog);
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
    glfwSetWindowSizeCallback(context->win, resizeCB);
}