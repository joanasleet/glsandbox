#ifndef STATE_H
#define STATE_H

#include "MatrixMath.h"
#include "Newton/include/Newton.h"

typedef struct {

    float accel;

    float up[3];
    float right[3];
    float forward[3];

    float position[3];
    float velocity[3];
    float targetVelocity[3];

    float torque[3];
    float orientation[4];
    float targetOrien[4];

    /* TODO: deprecate this */
    float angles[3];
    float targetAngles[3];
    float angleVelocity[3];

} State;

State *newState();

/* physics callback */
void applyForceAndTorque( const NewtonBody*, float, int );
void onTransform( const NewtonBody*, const float* const, int );

void applyTorque( State *state );
void calcPosition( vec3 target, State *src, float alpha);
void calcOrientation( quat target, State *src, float alpha);

#endif

