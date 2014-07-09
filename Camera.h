#ifndef CAMERA_H
#define	CAMERA_H

#include "common.h"
#include "GLProgram.h"
#include "GLContext.h"
#include "Context.h"
#include "ShaderProgram.h"

#define FOV (45.0f)
#define ASPECT_RATIO (4.0f / 3.0f)
#define NEAR_PLANE (0.1f)
#define FAR_PLANE (1000.0f)

#define CAM_SPEED 0.01

typedef struct Camera {
    float dirX;
    float dirY;
    float dirZ;

    double rotaX;
    double rotaY;
    double rotaZ;

    float xPos;
    float yPos;
    float zPos;

    float zspeed;
    float xspeed;

} Camera;

Camera* createCamera(float x, float y, float z);
void update(Camera* cam, Context* context, GLuint* shaders);

void cursorCB(GLFWwindow* win, double xpos, double ypos);
void scrollCB(GLFWwindow* win, double xoffset, double yoffset);
void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods);


#endif	/* CAMERA_H */

