#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

#include "State.h"
#include "MatrixMath.h"

#define FOV (75.0f)

#define ACCEL (0.01f)
#define TURN_SPEED (0.1f)

#define NEAR_PLANE (0.01f)
#define FAR_PLANE (10000.0f)
#define ASPECT_RATIO (16.0f / 9.0f)

typedef struct {

    /* control flags */
    uint8 wireframe;
    uint8 mouseGrab;

    /* translation & orientation */
    State *state;

    /* perspective params */
    float fov;
    float targetFov;
    float aspectRatio;

    /* perspective storage */
    float perspective[16];

} Camera;

Camera *newCamera(float x, float y, float z);
void freeCamera(Camera *cam);
void updateCam(Camera *cam);
void screenshot();

#endif


