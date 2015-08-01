#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

#include "State.h"
#include "MatrixMath.h"

extern float defaultFov;

typedef struct {

    float smoothing;
    float sensitivity;

    /* perspective params */
    float fov[2];
    float aspectRatio;

    /* clip planes */
    float nearClip;
    float farClip;

    /* perspective storage */
    float perspective[16];

    /* control flags */
    uint8 wireframe;
    uint8 mouseGrab;

    uint8 _pad[2];

    /* translation & orientation */
    State *state;

} Camera;

Camera *newCamera(float x, float y, float z);
void freeCamera(Camera *cam);
void updateCam(Camera *cam);
void screenshot();

#endif


