#include "GLCamera.h"


GLCamera::GLCamera(GLContext* context, GLProgram* program)
: context(context), program(program), zoom(-5.0) {

    
}

GLCamera::~GLCamera() {
}



void GLCamera::update() {

    double xpos, ypos;

    double midX = context->resX / 2;
    double midY = context->resY / 2;

    glfwGetCursorPos(context->main, &xpos, &ypos);
    glfwSetCursorPos(context->main, midX, midY);

    rotaX += (midX - xpos);
    rotaY += (midY - ypos);

    // Camera
    glm::mat4 yRotation = glm::rotate<float>(glm::mat4(), rotaY * CAM_SPEED, glm::vec3(-1, 0, 0));
    glm::mat4 xyRotation = glm::rotate<float>(yRotation, rotaX * CAM_SPEED, glm::vec3(0, 1, 0));

    // ModelView
    glm::mat4 modelView = glm::translate<float>(glm::mat4(), glm::vec3(0.0, 0.0, zoom));

    // Perspective
    glm::mat4 perspective = glm::perspective(FOV, SCREEN_RATIO, NEAR_PLANE, FAR_PLANE);

    // MVP
    glm::mat4 MVP = perspective * modelView*xyRotation;
    glUniformMatrix4fv(program->getVar("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
}