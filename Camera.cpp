#include "Camera.h"
#include "Context.h"
#include "Logger.h"
#include "Engine.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

extern Engine* renderer;

Camera* createCamera(float x, float y, float z) {
    Camera* cam = (Camera*) malloc(sizeof (Camera));

    cam->rotaX = 0.0f;
    cam->rotaY = 0.0f;
    cam->rotaZ = 0.0f;

    cam->dirX = 0.0f;
    cam->dirY = 0.0f;
    cam->dirZ = -1.0f;

    cam->xPos = x;
    cam->yPos = y;
    cam->zPos = z;

    cam->xspeed = 0.0f;
    cam->yspeed = 0.0f;
    cam->zspeed = 0.0f;

    cam->defaultSpeed = DEFAULT_CAM_SPEED;

    cam->mouseGrab = false;
    cam->wireframe = false;

    cam->perspective = (glm::mat4*) malloc(sizeof (glm::mat4));
    cam->orientation = (glm::mat4*) malloc(sizeof (glm::mat4));
    cam->translation = (glm::mat4*) malloc(sizeof (glm::mat4));

    return cam;
}

void update(Camera* cam) {

    // Camera Direction + Movement
    glm::mat4 yRotaB = glm::rotate<float>(glm::mat4(1.0), cam->rotaY * TURN_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRotaB = glm::rotate<float>(glm::mat4(1.0), cam->rotaX * TURN_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRotaB = glm::rotate<float>(glm::mat4(1.0), cam->rotaZ * TURN_SPEED, glm::vec3(0, 0, 1));

    glm::mat4 rotation = xRotaB * yRotaB * zRotaB;

    glm::vec4 camDir = rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    glm::vec4 strafe = rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

    cam->dirX = camDir.x;
    cam->dirY = camDir.y;
    cam->dirZ = camDir.z;

    cam->xPos += camDir.x * cam->zspeed + strafe.x * cam->xspeed;
    cam->yPos += camDir.y * cam->zspeed + strafe.y * cam->xspeed + cam->yspeed;
    cam->zPos += camDir.z * cam->zspeed + strafe.z * cam->xspeed;

    // Perspective + ModelView
    glm::mat4 yRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaY * TURN_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaX * TURN_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaZ * TURN_SPEED, glm::vec3(0, 0, 1));

    *(cam->orientation) = yRota * xRota * zRota;
    *(cam->translation) = glm::translate<float>(glm::mat4(1.0f), glm::vec3(-cam->xPos, -cam->yPos, -cam->zPos));
    *(cam->perspective) = glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE * 20.0f);
}

void cursorCB(GLFWwindow* win, double xpos, double ypos) {

    unsigned int xres = renderer->glContext->xRes;
    unsigned int yres = renderer->glContext->yRes;

    renderer->mainCam->rotaX += (xres / 2.0f) - xpos;
    renderer->mainCam->rotaY += (yres / 2.0f) - ypos;

    glfwSetCursorPos(win, (xres / 2.0f), (yres / 2.0f));
}

void cursorEnterCB(GLFWwindow* win, int enter) {
    if (enter == GL_TRUE) {

    }
}

void scrollCB(GLFWwindow* win, double xoffset, double yoffset) {

    Camera* cam = renderer->mainCam;

    if (cam->defaultSpeed > 1) {
        cam->defaultSpeed += 1 * yoffset;
    } else {
        cam->defaultSpeed += 0.1 * yoffset;
    }

    cam->defaultSpeed = MAX(cam->defaultSpeed, 0.1);
}

void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods) {

    Camera* cam = renderer->mainCam;

    static GLfloat inTessLvl = 2.0f;
    static GLfloat outTessLvl = 2.0f;

    bool wireframe = cam->wireframe;

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
                glfwSetCursorPos(win, renderer->glContext->xRes / 2.0f, renderer->glContext->yRes / 2.0f);
                glfwSetCursorPosCallback(win, cursorCB);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cam->mouseGrab = true;
            } else if (action == GLFW_PRESS && cam->mouseGrab) {
                glfwSetCursorPosCallback(win, NULL);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cam->mouseGrab = false;
            }
            break;
        case GLFW_KEY_V:
            if (action == GLFW_PRESS) {
                screenshot();
            }
            break;
        case GLFW_KEY_UP:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
                glUniform1f(glGetUniformLocation(1, "tessLevelInner"), ++inTessLvl);
            }
            break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
                glUniform1f(glGetUniformLocation(1, "tessLevelInner"), --inTessLvl);
            }
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
                glUniform1f(glGetUniformLocation(1, "tessLevelOuter"), --outTessLvl);
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
                glUniform1f(glGetUniformLocation(1, "tessLevelOuter"), ++outTessLvl);
            }
            break;
        case GLFW_KEY_F:

            if (action == GLFW_PRESS && !wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                cam->wireframe = true;
            } else if (action == GLFW_PRESS && wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                cam->wireframe = false;
            }
            break;
        default:
            return;
            break;
    }
}

void screenshot() {

    unsigned int x = renderer->glContext->xRes;
    unsigned int y = renderer->glContext->yRes;

    unsigned char* buffer = (unsigned char*) malloc(sizeof (unsigned char)*x * y * 3);
    glReadPixels(0, 0, x, y, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    char name[1024];
    long int t = time(NULL);
    sprintf(name, "screenshot_%ld.raw", t);

    FILE* target = fopen(name, "wb");
    if (!target) {
        ERR("Failed writing screenshot to <%s>", name);
    }

    int start_of_row;
    int bytes_in_row = x * 3;
    int bytes_left = x * y * 3;
    while (bytes_left > 0) {
        start_of_row = bytes_left - bytes_in_row;
        fwrite(&buffer[start_of_row], 1, bytes_in_row, target);
        bytes_left -= bytes_in_row;
    }
    fclose(target);
    free(buffer);

    char cmd[2048];
    sprintf(cmd, "convert -depth 8 -size %ix%i rgb:screenshot_%ld.raw screenshots/screenshot_%ld.png", x, y, t, t);

    system(cmd);
    sprintf(cmd, "rm -f %s", name);
    system(cmd);
}
