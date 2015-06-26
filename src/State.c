#include "Util.h"
#include "State.h"
#include "common.h"
#include "MatrixMath.h"

State *newState() {

    State *state = alloc( State, 1 );

    state->accel = 0.0f;

    nullVec( state->up );
    nullVec( state->right );
    nullVec( state->forward );

    nullVec( state->position );
    nullVec( state->velocity );
    nullVec( state->targetVelocity );

    nullVec( state->angles );
    nullVec( state->targetAngles );
    nullVec( state->angleVelocity );

    return state;
}

void setAngles(vec3 target, State *state, float alpha) {

    target[0] = state->angles[0] + state->angleVelocity[0] * alpha;
    target[1] = state->angles[1] + state->angleVelocity[1] * alpha;
    target[2] = state->angles[2] + state->angleVelocity[2] * alpha;
}

void setPosition(vec3 target, State *state, float alpha) {

    target[0] = state->position[0] + (state->forward[0] * state->velocity[2] + state->right[0] * state->velocity[0] + state->up[0] * state->velocity[1]) * alpha;
    target[1] = state->position[1] + (state->forward[1] * state->velocity[2] + state->right[1] * state->velocity[0] + state->up[1] * state->velocity[1]) * alpha;
    target[2] = state->position[2] + (state->forward[2] * state->velocity[2] + state->right[2] * state->velocity[0] + state->up[2] * state->velocity[1]) * alpha;
}
