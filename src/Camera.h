#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

#include "State.h"
#include "MatrixMath.h"

#define FOV (60.0f)

#define ACCEL (0.5f)
#define TURN_SPEED (0.1f)

#define NEAR_PLANE (0.1f)
#define FAR_PLANE (10000.0f)
#define ASPECT_RATIO (16.0f / 9.0f)

typedef struct {

    /* acceleration */
    float accel;

    /* facing direction */
    vec4 forward;

    /* control flags */
    uint8 wireframe;
    uint8 mouseGrab;

    /* contains translation
       params & orientation
       storage */
    State *state;

    /* orientation params */
    vec3 angles;

    /* perspective params */
    float fov;
    float aspectRatio;

    /* perspective storage */
    mat4 perspective;

    /* translation storage */
    mat4 translation;

} Camera;

Camera *newCamera(float x, float y, float z);
void freeCamera(Camera *cam);
void updateCam(Camera *cam);
void screenshot();

#endif

