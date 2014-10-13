#ifndef CAMERA_H
#define	CAMERA_H

#include "common.h"

#include "MatrixMath.h"

#define FOV (60.0f)
#define ASPECT_RATIO (16.0f / 9.0f)
#define NEAR_PLANE (0.1f)
#define FAR_PLANE (10000.0f)

#define TURN_SPEED 0.01f
#define DEFAULT_CAM_SPEED 0.5f

#define newCam() (createCamera(0, 10, 0))

typedef struct {

    vec3 speed;
    vec4 forward;
    vec3 position;

    // do i even need dis ?
    float defaultSpeed;

    uint8 mouseGrab;
    uint8 wireframe;
    
    vec3 angles;
    mat4 orientation;
    mat4 translation;
    mat4 perspective;

} Camera;

Camera* createCamera(float x, float y, float z);
void update(Camera* cam);

void cursorCB(GLFWwindow* win, double xpos, double ypos);
void cursorEnterCB(GLFWwindow* win, int enter);
void scrollCB(GLFWwindow* win, double xoffset, double yoffset);
void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods);

void screenshot();

#endif	/* CAMERA_H */

