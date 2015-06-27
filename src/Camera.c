#include "Camera.h"

#include "Util.h"
#include "Engine.h"
#include "Context.h"
#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

// needs to be some place else
static float baseUpVec[] = {0.0f, 1.0f, 0.0f};
static float baseRightVec[] = {1.0f, 0.0f, 0.0f};
static float baseForwardVec[] = {0.0f, 0.0f, -1.0f};

#define SMOOTHING (1.0 / 10.0)

Camera *newCamera(float x, float y, float z) {

    Camera *cam = alloc( Camera, 1 );

    exit_guard(cam);

    cam->state = newState();
    cam->state->position[0] = x;
    cam->state->position[1] = y;
    cam->state->position[2] = z;

    cam->state->accel = ACCEL;

    cam->mouseGrab = 0;
    cam->wireframe = 0;

    return cam;
}

void updateCam(Camera *cam) {

    State *state = cam->state;

    // calc & update angle velocity
    float newAngleVel[] = {
        lerpStep(state->angles[0], state->targetAngles[0], SMOOTHING),
        lerpStep(state->angles[1], state->targetAngles[1], SMOOTHING),
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
        state->velocity[0] + lerpStep(state->velocity[0], state->targetVelocity[0], SMOOTHING),
        state->velocity[1] + lerpStep(state->velocity[1], state->targetVelocity[1], SMOOTHING),
        state->velocity[2] + lerpStep(state->velocity[2], state->targetVelocity[2], SMOOTHING)
    };
    setVec3(state->velocity, newVel);
    setPosition(state->position, state, 1.0f);

    //watch("Position: (%.1f, %.1f, %.1f)\n\n", state->position[0], state->position[1], state->position[2]);
    //watch("Velocity: (%.2f, %.2f, %.2f)\n", newVel[0], newVel[1], newVel[2]);

    // calc & store perspective
    cam->fov += lerpStep(cam->fov, cam->targetFov, SMOOTHING);
    perspectiveInf(cam->perspective, NEAR_PLANE, cam->fov, cam->aspectRatio);
}

void freeCamera(Camera *cam) {

    free(cam->state);
    free(cam);
}

void screenshot() {

    unsigned int x = renderer->context->xRes;
    unsigned int y = renderer->context->yRes;

    unsigned char *buffer = malloc(sizeof (unsigned char) * x * y * 3);
    err_guard( buffer );
    glReadPixels(0, 0, x, y, GL_SRGB, GL_UNSIGNED_BYTE, buffer);

    char name[25];
    long int t = time(NULL);
    sprintf( name, "s%ld.raw", t );

    FILE *outputPNG = fopen(name, "wb");
    err_guard( outputPNG );

    int start_of_row;
    int bytes_in_row = x * 3;
    int bytes_left = x * y * 3;
    while (bytes_left > 0) {
        start_of_row = bytes_left - bytes_in_row;
        fwrite(&buffer[start_of_row], 1, bytes_in_row, outputPNG );
        bytes_left -= bytes_in_row;
    }
    fclose( outputPNG );
    free(buffer);

    char cmd[120];

    sprintf(cmd, "convert -depth 8 -size %ix%i rgb:s%ld.raw screenshots/s%ld.png", x, y, t, t);
    system(cmd);

    sprintf(cmd, "rm -f %s", name);
    system(cmd);
}

