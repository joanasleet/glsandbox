#include "Camera.h"
#include "Context.h"
#include "Debugger.h"
#include "Engine.h"

#include "SceneManager.h"

#include <time.h>

extern Engine* renderer;

Camera* createCamera(float x, float y, float z) {
    Camera* cam = (Camera*) malloc(sizeof (Camera));

    exit_guard(cam);

    cam->angles = vec3New(0, 0, 0);

    cam->position = vec3New(x, y, z);
    cam->forward = vec4New(0, 0, -1, 0);

    cam->speed = vec3New(0, 0, 0);

    cam->defaultSpeed = DEFAULT_CAM_SPEED;

    cam->mouseGrab = 0;
    cam->wireframe = 0;

    cam->perspective = mat4New();
    cam->orientation = mat4New();
    cam->translation = mat4New();

    return cam;
}

void update(Camera* cam) {

    float rotaCamX[4], rotaCamY[4], rotaCamZ[4];
    setQuat(rotaCamY, cam->angles[1] * TURN_SPEED, 1, 0, 0);
    setQuat(rotaCamX, cam->angles[0] * TURN_SPEED, 0, 1, 0);
    setQuat(rotaCamZ, cam->angles[2] * TURN_SPEED, 0, 0, 1);

    float rotaCamXYZ[4];
    multQ(rotaCamX, rotaCamY, rotaCamXYZ);
    multQ(rotaCamXYZ, rotaCamZ, rotaCamXYZ);
    float rotaMat[16];
    rotateQ(rotaMat, rotaCamXYZ);

    float forwardVec[] = {0, 0, -1, 0};
    float strafeVec[] = {1, 0, 0, 0};
    multMatVec(rotaMat, forwardVec, cam->forward);
    multMatVec(rotaMat, strafeVec, strafeVec);

    float forwardSpeed = cam->speed[2];
    float strafeSpeed = cam->speed[0];
    cam->position[0] += cam->forward[0] * forwardSpeed + strafeVec[0] * strafeSpeed;
    cam->position[1] += cam->forward[1] * forwardSpeed + strafeVec[1] * strafeSpeed + cam->speed[1];
    cam->position[2] += cam->forward[2] * forwardSpeed + strafeVec[2] * strafeSpeed;

    watch("Angles: (%f,\t%f,\t%f)\n", cam->angles[0], cam->angles[1], cam->angles[2]);
    watch("Position: (%f,\t%f,\t%f)\n", cam->position[0], cam->position[1], cam->position[2]);
    watch("\n");

    // reverse quat rotation
    rotaCamXYZ[1] *= -1.0f;
    rotaCamXYZ[2] *= -1.0f;
    rotaCamXYZ[3] *= -1.0f;

    rotateQ(cam->orientation, rotaCamXYZ);
    translate(cam->translation, -cam->position[0], -cam->position[1], -cam->position[2]); // faellt spaeter weg
    perspective(cam->perspective, 1, 10000, FOV, ASPECT_RATIO);
}

void cursorCB(GLFWwindow* win, double xpos, double ypos) {

    uint32 xres = renderer->glContext->xRes;
    uint32 yres = renderer->glContext->yRes;

    renderer->mainCam->angles[0] += (xres / 2.0f) - xpos;
    renderer->mainCam->angles[1] += (yres / 2.0f) - ypos;

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

    switch (key) {
        case GLFW_KEY_W:

            if (action == GLFW_PRESS) {
                cam->speed[2] = cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->speed[2] = 0.0f;
            } else {
                cam->speed[2] = cam->defaultSpeed;
            }
            break;
        case GLFW_KEY_A:

            if (action == GLFW_PRESS) {
                cam->speed[0] = -cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->speed[0] = 0.0f;
            }
            break;
        case GLFW_KEY_S:

            if (action == GLFW_PRESS) {
                cam->speed[2] = -cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->speed[2] = 0.0f;
            }
            break;
        case GLFW_KEY_D:

            if (action == GLFW_PRESS) {
                cam->speed[0] = cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->speed[0] = 0.0f;
            }
            break;
        case GLFW_KEY_SPACE:

            if (action == GLFW_PRESS) {
                cam->speed[1] = cam->defaultSpeed;
            } else if (action == GLFW_RELEASE) {
                cam->speed[1] = 0.0f;
            }
            break;
        case GLFW_KEY_C:

            if (action == GLFW_PRESS && !cam->mouseGrab) {
                glfwSetCursorPos(win, renderer->glContext->xRes / 2.0f, renderer->glContext->yRes / 2.0f);
                glfwSetCursorPosCallback(win, cursorCB);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cam->mouseGrab = 1;
            } else if (action == GLFW_PRESS && cam->mouseGrab) {
                glfwSetCursorPosCallback(win, NULL);
                glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cam->mouseGrab = 0;
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
            }
            break;
        case GLFW_KEY_DOWN:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
            }
            break;
        case GLFW_KEY_LEFT:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action == GLFW_PRESS) {
                glUseProgram(1);
            }
            break;
        case GLFW_KEY_F:

            if (action == GLFW_PRESS && !cam->wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                cam->wireframe = 1;
            } else if (action == GLFW_PRESS && cam->wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                cam->wireframe = 0;
            }
            break;
        case GLFW_KEY_R:

            if (action == GLFW_PRESS) {
                reloadScene(renderer);
            }
            break;
        case GLFW_KEY_M:

            if (action == GLFW_PRESS) {

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
        fprintf(stderr, "Failed to open file %s", name);
        exit(EXIT_FAILURE);
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
