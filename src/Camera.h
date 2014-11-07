#ifndef CAMERA_H
#define	CAMERA_H

#include "common.h"

#include "MatrixMath.h"

#define FOV (60.0f)
#define ASPECT_RATIO (16.0f / 9.0f)
#define NEAR_PLANE (0.1f)
#define FAR_PLANE (10000.0f)

#define TURN_SPEED (0.1f)
#define DEFAULT_CAM_SPEED (0.5f)

typedef struct {

    vec3 speed;
    vec4 forward;
    vec3 position;

    // do i even need dis ?
    float defaultSpeed;

    uint8 mouseGrab;
    uint8 wireframe;
    

    // replaces orientation
    vec3 angles;
    // no need to store this
    mat4 orientation;

    // can be derived from position
    mat4 translation;

    // update on init and callback
    mat4 perspective;
    // use:
    float fov;
    float aspectRatio;

} Camera;

Camera* newCamera(float x, float y, float z);
void update(Camera* cam);
void screenshot();

#endif	/* CAMERA_H */

