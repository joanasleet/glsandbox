#include "Camera.h"

#include "Engine.h"
#include "Context.h"
#include "Debugger.h"
#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

// needs to be some place else
static float baseUpVec[] = {0.0f, 1.0f, 0.0f, 0.0f};
static float baseRightVec[] = {1.0f, 0.0f, 0.0f, 0.0f};
static float baseForwardVec[] = {0.0f, 0.0f, -1.0f, 0.0f};

Camera *newCamera(float x, float y, float z) {

    Camera *cam = NEW(Camera);

    exit_guard(cam);

    cam->up = vec4New(0, 1, 0, 0);
    cam->right = vec4New(1, 0, 0, 0);
    cam->forward = vec4New(0, 0, -1, 0);

    cam->state = newState();
    cam->state->position[0] = x;
    cam->state->position[1] = y;
    cam->state->position[2] = z;

    cam->accel = ACCEL;

    cam->mouseGrab = 0;
    cam->wireframe = 0;

    cam->perspective = mat4New();

    return cam;
}

void updateCam(Camera *cam) {

    State *state = cam->state;

    state->angleVelocity[0] = lerpStep(state->angles[0], state->targetAngles[0], 1.0 / 10.0);
    state->angleVelocity[1] = lerpStep(state->angles[1], state->targetAngles[1], 1.0 / 10.0);
    state->angleVelocity[2] = 0.0f;

    state->angles[0] += state->angleVelocity[0];
    state->angles[1] += state->angleVelocity[1];
    state->angles[2] += state->angleVelocity[2];

    float rotaQ[4];
    rotate3D(rotaQ, state->angles);

    GLfloat rotation[16];
    rotateQ(rotation, rotaQ);

    // update orientation vectors
    multMatVec(rotation, baseUpVec, cam->up);
    multMatVec(rotation, baseRightVec, cam->right);
    multMatVec(rotation, baseForwardVec, cam->forward);

    // calc translation
    state->velocity[0] += lerpStep(state->velocity[0], state->targetVelocity[0], 1.0 / 10.0);
    state->velocity[1] += lerpStep(state->velocity[1], state->targetVelocity[1], 1.0 / 10.0);
    state->velocity[2] += lerpStep(state->velocity[2], state->targetVelocity[2], 1.0 / 10.0);

    PULSE(watch("%s\n", "- - - - - - - - - - - - -"), 10);
    PULSE(watch("Velocity (%f, %f, %f)\n", state->velocity[0], state->velocity[1], state->velocity[2]), 10);
    PULSE(watch("%s\n\n\n", "- - - - - - - - - - - - -"), 10);

    float strafeSpeed = state->velocity[0];
    float verticalSpeed = state->velocity[1];
    float forwardSpeed = state->velocity[2];

    state->position[0] += (cam->forward[0] * forwardSpeed + cam->right[0] * strafeSpeed + cam->up[0] * verticalSpeed);
    state->position[1] += (cam->forward[1] * forwardSpeed + cam->right[1] * strafeSpeed + cam->up[1] * verticalSpeed);
    state->position[2] += (cam->forward[2] * forwardSpeed + cam->right[2] * strafeSpeed + cam->up[2] * verticalSpeed);

    // calc & store perspective
    perspectiveInf(cam->perspective, NEAR_PLANE, cam->fov, cam->aspectRatio);
}

void freeCamera(Camera *cam) {

    free(cam->up);
    free(cam->right);
    free(cam->forward);

    freeState(cam->state);

    free(cam->perspective);

    free(cam);
}

void screenshot() {

    unsigned int x = renderer->context->xRes;
    unsigned int y = renderer->context->yRes;

    unsigned char *buffer = (unsigned char *) malloc(sizeof (unsigned char) * x * y * 3);
    glReadPixels(0, 0, x, y, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    char name[1024];
    long int t = time(NULL);
    sprintf(name, "screenshot_%ld.raw", t);

    FILE *target = fopen(name, "wb");
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
