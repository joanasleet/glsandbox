#include "Camera.h"
#include "Context.h"

Camera* createCamera(float x, float y, float z) {
    Camera* cam = (Camera*) malloc(sizeof (Camera));

    cam->rotaX = 0.0f;
    cam->rotaY = 0.0f;
    cam->rotaZ = 0.0f;

    cam->xPos = x;
    cam->yPos = y;
    cam->zPos = z;

    cam->xspeed = 0.0f;
    cam->zspeed = 0.0f;

    cam->zRotaSpeed = 0.0f;

    return cam;
}

void update(Camera* cam, Context* context, GLuint prog) {

    double xpos, ypos;

    double midX = context->xRes / 2;
    double midY = context->yRes / 2;

    cam->zPos += -cam->zspeed;
    cam->xPos += cam->xspeed;

    cam->rotaZ += cam->zRotaSpeed;

    glfwGetCursorPos(context->win, &xpos, &ypos);
    glfwSetCursorPos(context->win, midX, midY);

    cam->rotaX += (midX - xpos);
    cam->rotaY += (midY - ypos);

    glm::mat4 translateCamera = glm::translate<float>(glm::mat4(1.0f), glm::vec3(-cam->xPos, -cam->yPos, -cam->zPos));

    glm::mat4 yRota = glm::rotate<float>(glm::mat4(1.0f), cam->rotaY * CAM_SPEED, glm::vec3(-1, 0, 0));
    glm::mat4 xRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaX * CAM_SPEED, glm::vec3(0, 1, 0));
    glm::mat4 zRota = glm::rotate<float>(glm::mat4(1.0f), -cam->rotaZ * CAM_SPEED * 10, glm::vec3(0, 0, 1));

    glm::mat4 perspective = glm::infinitePerspective(FOV, ASPECT_RATIO, NEAR_PLANE);


    // MVP: move around camera (0, 0, 0)
    glm::mat4 MVP = perspective * yRota * xRota * zRota * translateCamera;
    glUniformMatrix4fv(glGetUniformLocation(prog, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
}