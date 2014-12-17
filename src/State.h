#ifndef STATE_H
#define STATE_H

#include "MatrixMath.h"

typedef struct {

    vec3 up;
    vec3 right;
    vec3 forward;

    float accel;

    vec3 position;
    vec3 velocity;
    vec3 targetVelocity;

    vec3 angles;
    vec3 targetAngles;
    vec3 angleVelocity;

} State;

State *newState();
void freeState(State *state);

void setAngles(vec3 target, State *src, float alpha);
void setPosition(vec3 target, State *src, float alpha);

void setVelocity(State *state);
void setAngleVelocity(State *state);


#endif