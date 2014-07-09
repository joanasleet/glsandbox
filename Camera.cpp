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

void update(Camera* cam, Context* context, GLuint* shaders) {

    //cam->zPos += -cam->zspeed;
    //cam->xPos += cam->xspeed;

    glm::mat4 yRotaB = glm::rotate<float>(glm::mat4(1.0f), cam->rotaY * CAM_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRotaB = glm::rotate<float>(glm::mat4(1.0f), cam->rotaX * CAM_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRotaB = glm::rotate<float>(glm::mat4(1.0f), cam->rotaZ * CAM_SPEED * 10, glm::vec3(0, 0, 1));

    glm::vec4 camDir = xRotaB * yRotaB * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    
    cam->xPos += camDir.x * cam->zspeed;
    cam->yPos += camDir.y * cam->zspeed;
    cam->zPos += camDir.z * cam->zspeed;

    glm::vec3 translVec = glm::vec3(-cam->xPos, -cam->yPos, -cam->zPos);

    glm::mat4 translateCamera = glm::translate<float>(glm::mat4(1.0f), translVec);
    glm::mat4 yRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaY * CAM_SPEED, glm::vec3(1, 0, 0));
    glm::mat4 xRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaX * CAM_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaZ * CAM_SPEED * 10, glm::vec3(0, 0, 1));
    glm::mat4 perspective = glm::infinitePerspective(FOV, ASPECT_RATIO, NEAR_PLANE);

    glm::mat4 MVP = perspective * yRota * xRota * zRota * translateCamera;
    glUniformMatrix4fv(glGetUniformLocation(shaders[0], "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));


    //MVP = perspective * yRota * xRota * zRota;

    glUniformMatrix4fv(glGetUniformLocation(shaders[1], "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
}