#ifndef GLCAMERA_H
#define	GLCAMERA_H

#include "common.h"
#include "GLProgram.h"
#include "GLContext.h"

#define FOV (45.0f)
#define SCREEN_RATIO (4.0f / 3.0f)
#define NEAR_PLANE (0.1f)
#define FAR_PLANE (100.0f)

#define CAM_SPEED 0.01


class GLCamera {
public:
    double rotaX;
    double rotaY;
    
    double zoom;
    
    GLContext* context;
    GLProgram* program;
    
    GLCamera(GLContext* context, GLProgram* program);
    
    void update();
        
    virtual ~GLCamera();
private:

};
#endif	/* GLCAMERA_H */
