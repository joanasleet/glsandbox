#include "Camera.h"

#include "Engine.h"
#include "Context.h"
#include "Debugger.h"
#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

Camera *newCamera(float x, float y, float z) {

    Camera *cam = NEW(Camera);

    exit_guard(cam);

    cam->forward = vec4New(0, 0, -1, 0);
    cam->angles = vec3New(0, 0, 0);

    cam->state = newState();
    cam->state->position[0] = x;
    cam->state->position[1] = y;
    cam->state->position[2] = z;

    cam->accel = ACCEL;

    cam->mouseGrab = 0;
    cam->wireframe = 0;

    cam->perspective = mat4New();
    cam->translation = mat4New();

    return cam;
}

void updateCam(Camera *cam) {

    // calc orientation quat
    float rotaCamX[4], rotaCamY[4], rotaCamZ[4];
    setQuat(rotaCamY, cam->angles[1] * TURN_SPEED, 1, 0, 0);
    setQuat(rotaCamX, cam->angles[0] * TURN_SPEED, 0, 1, 0);
    setQuat(rotaCamZ, cam->angles[2] * TURN_SPEED, 0, 0, 1);

    float rotaCamXY[4];
    multQ(rotaCamX, rotaCamY, rotaCamXY);
    float rotaCamXYZ[4];
    multQ(rotaCamXY, rotaCamZ, rotaCamXYZ);

    // convert orientation quat to matrix
    float rotaMat[16];
    rotateQ(rotaMat, rotaCamXYZ);

    // update orientation vectors
    float strafeVec[] = {1, 0, 0, 0};
    float forwardVec[] = {0, 0, -1, 0};

    float strafe[4];
    multMatVec(rotaMat, forwardVec, cam->forward);
    multMatVec(rotaMat, strafeVec, strafe);

    // calc translation
    State *state = cam->state;
    float strafeSpeed = state->velocity[0];
    float verticalSpeed = state->velocity[1];
    float forwardSpeed = state->velocity[2];
    state->position[0] += (cam->forward[0] * forwardSpeed + strafe[0] * strafeSpeed);
    state->position[1] += (cam->forward[1] * forwardSpeed + strafe[1] * strafeSpeed + verticalSpeed);
    state->position[2] += (cam->forward[2] * forwardSpeed + strafe[2] * strafeSpeed);

    // store orientation
    quat orientation = cam->state->orientation;
    orientation[0] = -rotaCamXYZ[0];
    orientation[1] = -rotaCamXYZ[1];
    orientation[2] = -rotaCamXYZ[2];
    orientation[3] = rotaCamXYZ[3];

    // calc & store perspective
    perspectiveInf(cam->perspective, NEAR_PLANE, cam->fov, cam->aspectRatio);

    // store translation
    translate(cam->translation, -state->position[0], -state->position[1], -state->position[2]);
}

void freeCamera(Camera *cam) {

    free(cam->forward);
    freeState(cam->state);
    free(cam->angles);
    free(cam->perspective);
    free(cam->translation);
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
