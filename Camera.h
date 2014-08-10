#ifndef CAMERA_H
#define	CAMERA_H

#include "common.h"
#include <glm/glm.hpp>

#define FOV (45.0f)
#define ASPECT_RATIO (4.0f / 3.0f)
#define NEAR_PLANE (0.1f)
#define FAR_PLANE (1000.0f)

#define TURN_SPEED 0.01f
#define DEFAULT_CAM_SPEED 0.5f

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

    float xspeed;
    float yspeed;
    float zspeed;

    float defaultSpeed;

    bool mouseGrab;
    bool wireframe;

    glm::mat4* perspective;
    glm::mat4* orientation;
    glm::mat4* translation;

} Camera;

Camera* createCamera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
void update(Camera* cam);

void cursorCB(GLFWwindow* win, double xpos, double ypos);
void cursorEnterCB(GLFWwindow* win, int enter);
void scrollCB(GLFWwindow* win, double xoffset, double yoffset);
void keyCB(GLFWwindow* win, int key, int scancode, int action, int mods);

void screenshot();

#endif	/* CAMERA_H */

