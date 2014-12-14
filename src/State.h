#ifndef STATE_H
#define STATE_H

#include "MatrixMath.h"

typedef struct {

    vec3 position;
    vec3 velocity;
    vec3 targetVelocity;

    vec3 angles;
    vec3 targetAngles;
    vec3 angleVelocity;

} State;

State *newState();
void freeState(State *state);

#endif