#include "Camera.h"

#include "Engine.h"
#include "Context.h"
#include "Debugger.h"
#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

// needs to be some place else
static float baseUpVec[] = {0.0f, 1.0f, 0.0f};
static float baseRightVec[] = {1.0f, 0.0f, 0.0f};
static float baseForwardVec[] = {0.0f, 0.0f, -1.0f};

#define SMOOTH_FACTOR (1.0 / 10.0)

Camera *newCamera(float x, float y, float z) {

    Camera *cam = NEW(Camera);

    exit_guard(cam);

    cam->state = newState();
    cam->state->position[0] = x;
    cam->state->position[1] = y;
    cam->state->position[2] = z;

    cam->state->accel = ACCEL;

    cam->mouseGrab = 0;
    cam->wireframe = 0;

    cam->perspective = mat4New();

    return cam;
}

void updateCam(Camera *cam) {

    State *state = cam->state;

    // calc & update angle velocity
    float newAngleVel[] = {
        lerpStep(state->angles[0], state->targetAngles[0], SMOOTH_FACTOR),
        lerpStep(state->angles[1], state->targetAngles[1], SMOOTH_FACTOR),
        0.0f
    };
    setVec3(state->angleVelocity, newAngleVel);
    setAngles(state->angles, state, 1.0f);

    // calc rotation
    float rotaQuaternion[4];
    rotate3D(rotaQuaternion, state->angles);
    GLfloat rotaMatrix[16];
    rotateQ(rotaMatrix, rotaQuaternion);

    // update orientation vectors
    multMatVec3(rotaMatrix, baseUpVec, state->up);
    multMatVec3(rotaMatrix, baseRightVec, state->right);
    multMatVec3(rotaMatrix, baseForwardVec, state->forward);

    // calc & update velocity
    float newVel[] = {
        state->velocity[0] + lerpStep(state->velocity[0], state->targetVelocity[0], SMOOTH_FACTOR),
        state->velocity[1] + lerpStep(state->velocity[1], state->targetVelocity[1], SMOOTH_FACTOR),
        state->velocity[2] + lerpStep(state->velocity[2], state->targetVelocity[2], SMOOTH_FACTOR)
    };
    setVec3(state->velocity, newVel);
    setPosition(state->position, state, 1.0f);

    watch("Position: (%.1f, %.1f, %.1f)\n\n", state->position[0], state->position[1], state->position[2]);
    //watch("Velocity: (%.2f, %.2f, %.2f)\n", newVel[0], newVel[1], newVel[2]);

    // calc & store perspective
    cam->fov += lerpStep(cam->fov, cam->targetFov, SMOOTH_FACTOR);
    perspectiveInf(cam->perspective, NEAR_PLANE, cam->fov, cam->aspectRatio);
}

void freeCamera(Camera *cam) {

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
