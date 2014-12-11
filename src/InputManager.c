#include "Engine.h"
#include "Debugger.h"
#include "InputManager.h"
#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

void cursorCB(GLFWwindow *win, double xpos, double ypos) {

    static double xwrap = 0.0;

    uint32 xres = renderer->context->xRes;
    uint32 yres = renderer->context->yRes;

    xwrap += (xres / 2.0f) - xpos; // angle velocity
    if (xwrap >= 360.0) xwrap -= 360.0;
    if (xwrap <= -360.0) xwrap += 360.0;

    State *camState = renderer->mainCam->state;
    camState->angleVelocity[0] = ((xres / 2.0f) - xpos) * 0.2f;
    camState->angleVelocity[1] = ((yres / 2.0f) - ypos) * 0.2f;

    // camState->angles[0] += (xres / 2.0f) - xpos;
    // camState->angles[1] += (yres / 2.0f) - ypos;

    glfwSetCursorPos(win, (xres / 2.0f), (yres / 2.0f));
}

void cursorEnterCB(GLFWwindow *win, int enter) {
    if (enter == GL_TRUE) {

    }
}

void scrollCB(GLFWwindow *win, double xoffset, double yoffset) {


    Camera *cam = renderer->mainCam;

    if (cam->accel > 1.0f) {
        cam->accel += 1.0f * yoffset;
    } else if (cam->accel > 0.1f) {
        cam->accel += 0.1f * yoffset;
    } else {
        cam->accel += 0.01f * yoffset;
    }

    cam->accel = MAX(cam->accel, 0.01f);
}

void keyCB(GLFWwindow *win, int key, int scancode, int action, int mods) {

    Camera *cam = renderer->mainCam;

    switch (key) {
    case GLFW_KEY_W:

        if (action == GLFW_PRESS) {
            cam->state->velocity[2] = cam->accel;
        } else if (action == GLFW_RELEASE) {
            cam->state->velocity[2] = 0.0f;
        } else {
            cam->state->velocity[2] = cam->accel;
        }
        break;
    case GLFW_KEY_A:

        if (action == GLFW_PRESS) {
            cam->state->velocity[0] = -cam->accel;
        } else if (action == GLFW_RELEASE) {
            cam->state->velocity[0] = 0.0f;
        } else {
            cam->state->velocity[0] = -cam->accel;
        }
        break;
    case GLFW_KEY_S:

        if (action == GLFW_PRESS) {
            cam->state->velocity[2] = -cam->accel;
        } else if (action == GLFW_RELEASE) {
            cam->state->velocity[2] = 0.0f;
        } else {
            cam->state->velocity[2] = -cam->accel;
        }
        break;
    case GLFW_KEY_D:

        if (action == GLFW_PRESS) {
            cam->state->velocity[0] = cam->accel;
        } else if (action == GLFW_RELEASE) {
            cam->state->velocity[0] = 0.0f;
        } else {
            cam->state->velocity[0] = cam->accel;
        }
        break;
    case GLFW_KEY_SPACE:

        if (action == GLFW_PRESS) {
            cam->state->velocity[1] = cam->accel;
        } else if (action == GLFW_RELEASE) {
            cam->state->velocity[1] = 0.0f;
        }
        break;
    case GLFW_KEY_C:

        if (action == GLFW_PRESS && !cam->mouseGrab) {
            glfwSetCursorPos(win, renderer->context->xRes / 2.0f, renderer->context->yRes / 2.0f);
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
            cam->state->angleVelocity[1] = cam->accel * 10.0;
        } else if (action == GLFW_RELEASE) {
            cam->state->angleVelocity[1] = 0.0f;
        }
        break;
    case GLFW_KEY_DOWN:
        if (action == GLFW_PRESS) {
            cam->state->angleVelocity[1] = -cam->accel * 10.0;
        } else if (action == GLFW_RELEASE) {
            cam->state->angleVelocity[1] = 0.0f;
        }
        break;
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS) {
            cam->state->angleVelocity[0] = cam->accel * 10.0;
        } else if (action == GLFW_RELEASE) {
            cam->state->angleVelocity[0] = 0.0f;
        }
        break;
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS) {
            cam->state->angleVelocity[0] = -cam->accel * 10.0;
        } else if (action == GLFW_RELEASE) {
            cam->state->angleVelocity[0] = 0.0f;
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
    case GLFW_KEY_Z:

        if (action == GLFW_PRESS) {
            cam->fov += 10.0f;
        } else if (action == GLFW_RELEASE) {
            cam->fov -= 10.0f;
        }
        break;
    case GLFW_KEY_E:

        if (action == GLFW_PRESS) {
            cam->state->angleVelocity[0] = 0.0f;
            cam->state->angleVelocity[1] = 0.0f;
            cam->state->angleVelocity[2] = 0.0f;
        }
        break;
    default:
        return;
        break;
    }
}