#include "Util.h"
#include "Engine.h"
#include "Timestep.h"
#include "InputManager.h"
#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

void cursorCB(GLFWwindow *win, double xpos, double ypos) {

    uint32 xres = renderer->context->xRes;
    uint32 yres = renderer->context->yRes;

    State *state = renderer->mainCam->state;

    state->torque[0] += ((xres / 2.0) - xpos) * renderer->mainCam->sensitivity;
    state->torque[1] += ((yres / 2.0) - ypos) * renderer->mainCam->sensitivity;

    glfwSetCursorPos(win, (xres / 2.0f), (yres / 2.0f));
}

void cursorEnterCB(GLFWwindow *win, int enter) {
    if (enter == GL_TRUE) {

    }
}

void scrollCB(GLFWwindow *win, double xoffset, double yoffset) {

    State *camState = renderer->mainCam->state;
    camState->accel *= pow( 1.25f, yoffset );
}

extern float force[3], torque[3];
void keyCB(GLFWwindow *win, int key, int scancode, int action, int mods) {

    Camera *cam = renderer->mainCam;
    State *camState = cam->state;

    switch (key) {

    // move camera
    case GLFW_KEY_W:

        if (action == GLFW_PRESS) {
            camState->targetVelocity[2] = camState->accel;
            //cam->targetFov = FOV + 15.0f;
        } else if (action == GLFW_RELEASE) {
            camState->targetVelocity[2] = 0.0f;
            //cam->targetFov = FOV;
        } else {
            camState->targetVelocity[2] = camState->accel;
        }
        break;
    case GLFW_KEY_A:

        if (action == GLFW_PRESS) {
            camState->targetVelocity[0] = -camState->accel;
        } else if (action == GLFW_RELEASE) {
            camState->targetVelocity[0] = 0.0f;
        } else {
            camState->targetVelocity[0] = -camState->accel;
        }
        break;
    case GLFW_KEY_S:

        if (action == GLFW_PRESS) {
            camState->targetVelocity[2] = -camState->accel;
        } else if (action == GLFW_RELEASE) {
            camState->targetVelocity[2] = 0.0f;
        } else {
            camState->targetVelocity[2] = -camState->accel;
        }
        break;
    case GLFW_KEY_D:

        if (action == GLFW_PRESS) {
            camState->targetVelocity[0] = camState->accel;
        } else if (action == GLFW_RELEASE) {
            camState->targetVelocity[0] = 0.0f;
        } else {
            camState->targetVelocity[0] = camState->accel;
        }
        break;
    case GLFW_KEY_SPACE:

        if (action == GLFW_PRESS) {
            camState->targetVelocity[1] = camState->accel;
        } else if (action == GLFW_RELEASE) {
            camState->targetVelocity[1] = 0.0f;
        }
        break;
    case GLFW_KEY_X:

        if (action == GLFW_PRESS) {
            camState->targetVelocity[1] = -camState->accel;
        } else if (action == GLFW_RELEASE) {
            camState->targetVelocity[1] = 0.0f;
        }
        break;

    // move some object
    case GLFW_KEY_UP:
        if (action == GLFW_PRESS) {
            force[1] = 200.0f;
        } else if (action == GLFW_RELEASE) {
            force[1] = 0.0f;
        }
        break;
    case GLFW_KEY_DOWN:
        if (action == GLFW_PRESS) {
            torque[1] = 500.0f;
        } else if (action == GLFW_RELEASE) {
            torque[1] = 0.0f;
        }
        break;
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS) {
            force[0] = -200.0f;
        } else if (action == GLFW_RELEASE) {
            force[0] = 0.0f;
        }
        break;
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS) {
            force[0] = 200.0f;
        } else if (action == GLFW_RELEASE) {
            force[0] = 0.0f;
        }
        break;

    // roll camera
    case GLFW_KEY_E:

        if (action == GLFW_PRESS) {
            cam->state->torque[2] += 2.0f;
        } else if (action == GLFW_RELEASE) {
            /* nothing */
        } else {
            cam->state->torque[2] += 2.0f;
        }
        break;

    case GLFW_KEY_Q:

        if (action == GLFW_PRESS) {
            cam->state->torque[2] -= 2.0f;
        } else if (action == GLFW_RELEASE) {
            /* nothing */
        } else {
            cam->state->torque[2] -= 2.0f;
        }
        break;

    // zoom in
    case GLFW_KEY_F:

        if (action == GLFW_PRESS) {
            cam->fov[1] = defaultFov - 50.0f;
        } else if (action == GLFW_RELEASE) {
            cam->fov[1] = defaultFov;

        }
        break;

    // grab mouse cursor
    case GLFW_KEY_C:

        if (action == GLFW_PRESS && !cam->mouseGrab) {
            glfwSetCursorPos(win, renderer->context->xRes/2.0f, renderer->context->yRes/2.0f);
            glfwSetCursorPosCallback(win, cursorCB);
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cam->mouseGrab = 1;
        } else if (action == GLFW_PRESS && cam->mouseGrab) {
            glfwSetCursorPosCallback(win, NULL);
            glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cam->mouseGrab = 0;
        }
        break;

    // take screenshot
    case GLFW_KEY_V:
        if (action == GLFW_PRESS) {
            screenshot();
        }
        break;

    // reload scene
    case GLFW_KEY_R:

        if (action == GLFW_PRESS) {
            reloadScene(renderer);
        }
        break;

    // reload scene
    case GLFW_KEY_T:

        if (action == GLFW_PRESS) {
            pauseGlobalTime();
        }
        break;

    // toggle face culling
    case GLFW_KEY_1:

        if (action == GLFW_PRESS) {
            glDisable(GL_CULL_FACE);
        } else if (action == GLFW_RELEASE) {
            glEnable(GL_CULL_FACE);
        }
        break;

    // toggle wireframe mode
    case GLFW_KEY_2:

        if (action == GLFW_PRESS && !cam->wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            cam->wireframe = 1;
        } else if (action == GLFW_PRESS && cam->wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            cam->wireframe = 0;
        }
        break;

    default:
        return;
        break;
    }
}
