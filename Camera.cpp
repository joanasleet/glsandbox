#include "Camera.h"
#include "Context.h"

Camera* createCamera(float x, float y, float z) {
    Camera* cam = (Camera*) malloc(sizeof (Camera));

    cam->rotaX = 0.0f;
    cam->rotaY = 0.0f;
    cam->rotaZ = 0.0f;

    cam->dirX = 0.0f;
    cam->dirY = 0.0f;
    cam->dirZ = -1.0f;

    cam->xPos = x;
    cam->yPos = y;
    cam->zPos = z;

    cam->xspeed = 0.0f;
    cam->zspeed = 0.0f;

    return cam;
}

void update(Camera* cam) {

    glm::mat4 yRotaB = glm::rotate<float>(glm::mat4(1.0), cam->rotaY * CAM_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRotaB = glm::rotate<float>(glm::mat4(1.0), cam->rotaX * CAM_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRotaB = glm::rotate<float>(glm::mat4(1.0), cam->rotaZ * CAM_SPEED * 10, glm::vec3(0, 0, 1));

    glm::vec4 camDir = xRotaB * yRotaB * zRotaB * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    cam->dirX = camDir.x;
    cam->dirY = camDir.y;
    cam->dirZ = camDir.z;

    cam->xPos += camDir.x * cam->zspeed;
    cam->yPos += camDir.y * cam->zspeed;
    cam->zPos += camDir.z * cam->zspeed;
}