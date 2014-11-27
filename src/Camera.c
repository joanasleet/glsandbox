#include "Camera.h"
#include "Context.h"
#include "Debugger.h"
#include "Engine.h"

#include "SceneManager.h"

#include <time.h>

extern Engine *renderer;

Camera *newCamera(float x, float y, float z) {
    Camera *cam = (Camera *) malloc(sizeof (Camera));

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

void update(Camera *cam, double dt) {

    // dt = (float) dt;

    float rotaCamX[4], rotaCamY[4], rotaCamZ[4];
    setQuat(rotaCamY, cam->angles[1] * TURN_SPEED, 1, 0, 0);
    setQuat(rotaCamX, cam->angles[0] * TURN_SPEED, 0, 1, 0);
    setQuat(rotaCamZ, cam->angles[2] * TURN_SPEED, 0, 0, 1);

    float rotaCamXY[4];
    multQ(rotaCamX, rotaCamY, rotaCamXY);
    float rotaCamXYZ[4];
    multQ(rotaCamXY, rotaCamZ, rotaCamXYZ);
    float rotaMat[16];
    rotateQ(rotaMat, rotaCamXYZ);

    float forwardVec[] = {0, 0, -1, 0};
    float rightVec[] = {1, 0, 0, 0};
    float strafeVec[4];
    multMatVec(rotaMat, forwardVec, cam->forward);
    multMatVec(rotaMat, rightVec, strafeVec);

    float forwardSpeed = cam->speed[2];
    float strafeSpeed = cam->speed[0];
    cam->position[0] += (cam->forward[0] * forwardSpeed + strafeVec[0] * strafeSpeed) * dt;
    cam->position[1] += (cam->forward[1] * forwardSpeed + strafeVec[1] * strafeSpeed + cam->speed[1]) * dt;
    cam->position[2] += (cam->forward[2] * forwardSpeed + strafeVec[2] * strafeSpeed) * dt;

    invertQ(rotaCamXYZ);

    rotateQ(cam->orientation, rotaCamXYZ);
    translate(cam->translation, -cam->position[0], -cam->position[1], -cam->position[2]); // faellt spaeter weg
    perspectiveInf(cam->perspective, 1.0f, cam->fov, cam->aspectRatio);
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
