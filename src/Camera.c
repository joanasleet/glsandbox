#include <time.h>
#include "Util.h"
#include "Engine.h"
#include "Camera.h"
#include "Context.h"
#include "SceneManager.h"

extern Engine *renderer;

// needs to be some place else
static float baseUpVec[] = {0.0f, 1.0f, 0.0f};
static float baseRightVec[] = {1.0f, 0.0f, 0.0f};
static float baseForwardVec[] = {0.0f, 0.0f, -1.0f};

float defaultFov;

Camera *newCamera(float x, float y, float z) {

    Camera *cam = alloc( Camera, 1 );

    exit_guard(cam);

    cam->state = newState();
    cam->state->position[0] = x;
    cam->state->position[1] = y;
    cam->state->position[2] = z;

    cam->state->up[0] = 0.0f;
    cam->state->up[1] = 1.0f;
    cam->state->up[2] = 0.0f;

    cam->state->right[0] = 1.0f;
    cam->state->right[1] = 0.0f;
    cam->state->right[2] = 0.0f;

    cam->state->forward[0] = 0.0f;
    cam->state->forward[1] = 0.0f;
    cam->state->forward[2] = -1.0f;

    cam->state->orientation[0] = 1.0f;
    cam->state->orientation[1] = 0.0f;
    cam->state->orientation[2] = 0.0f;
    cam->state->orientation[3] = 0.0f;

    cam->mouseGrab = 0;
    cam->wireframe = 0;

    return cam;
}

void updateCam(Camera *cam) {

    State *state = cam->state;

    /* calc & update angle velocity */
    float newAngleVel[] = {
        lerpStep(state->angles[0], state->targetAngles[0], cam->smoothing ),
        lerpStep(state->angles[1], state->targetAngles[1], cam->smoothing ),
        lerpStep(state->angles[2], state->targetAngles[2], cam->smoothing )
    };
    cpyBuf( state->angleVelocity, newAngleVel, 3 );
    calcAngles( state->angles, state, 1.0f );
    calcOrientation( state->orientation, state, 1.0f );

    // calc rotation
    GLfloat rotaMatrix[16];
    quatToMat(rotaMatrix, state->orientation);

    // update orientation vectors
    float up[3];
    mat4multVec3(rotaMatrix, baseUpVec, up );
    cpyBuf( state->up, up, 3 );

    float right[3];
    mat4multVec3(rotaMatrix, baseRightVec, right );
    cpyBuf( state->right, right, 3 );

    float forward[3];
    mat4multVec3(rotaMatrix, baseForwardVec, forward );
    cpyBuf( state->forward, forward, 3 );

    // calc & update velocity
    float newVel[] = {
        state->velocity[0] + lerpStep(state->velocity[0], state->targetVelocity[0], cam->smoothing ),
        state->velocity[1] + lerpStep(state->velocity[1], state->targetVelocity[1], cam->smoothing ),
        state->velocity[2] + lerpStep(state->velocity[2], state->targetVelocity[2], cam->smoothing )
    };
    cpyBuf( state->velocity, newVel, 3 );
    calcPosition( state->position, state, 1.0f);

    // calc & store perspective
    cam->fov[0] += lerpStep( cam->fov[0], cam->fov[1], cam->smoothing );
    mat4perspinf(cam->perspective, cam->nearClip, cam->fov[0], cam->aspectRatio);
}

void freeCamera(Camera *cam) {

    free(cam->state);
    free(cam);
}

void screenshot() {

    unsigned int x = renderer->context->xRes;
    unsigned int y = renderer->context->yRes;

    unsigned char *buffer = malloc(sizeof (unsigned char) * x * y * 3);
    err_guard( buffer );
    glReadPixels(0, 0, x, y, GL_RGB, GL_UNSIGNED_BYTE, buffer);

    char name[25];
    long int t = time(NULL);
    sprintf( name, "s%ld.raw", t );

    FILE *outputPNG = fopen(name, "wb");
    err_guard( outputPNG );

    int start_of_row;
    int bytes_in_row = x * 3;
    int bytes_left = x * y * 3;
    while (bytes_left > 0) {
        start_of_row = bytes_left - bytes_in_row;
        fwrite(&buffer[start_of_row], 1, bytes_in_row, outputPNG );
        bytes_left -= bytes_in_row;
    }
    fclose( outputPNG );
    free(buffer);

    char cmd[120];

    sprintf(cmd, "convert -depth 8 -size %ix%i rgb:s%ld.raw screenshots/s%ld.png", x, y, t, t);
    system(cmd);

    sprintf(cmd, "rm -f %s", name);
    system(cmd);
}

