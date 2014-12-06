#ifndef STATE_H
#define STATE_H

#include "MatrixMath.h"

typedef struct {

    vec3 position;
    vec3 velocity;
    quat orientation;

} State;

State *newState();
void freeState(State *state);

#endif