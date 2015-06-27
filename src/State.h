#ifndef STATE_H
#define STATE_H

#include "MatrixMath.h"

typedef struct {

    float accel;

    float up[3];
    float right[3];
    float forward[3];

    float position[3];
    float velocity[3];
    float targetVelocity[3];

    // TODO: use quaternion and multiply in deltas
    float angles[3];
    float targetAngles[3];
    float angleVelocity[3];
} State;

State *newState();

void setAngles(vec3 target, State *src, float alpha);
void setPosition(vec3 target, State *src, float alpha);

void setVelocity(State *state);
void setAngleVelocity(State *state);

#endif

