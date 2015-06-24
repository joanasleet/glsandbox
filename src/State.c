#include "Util.h"
#include "State.h"
#include "common.h"

State *newState() {

    State *state = alloc( State, 1 );

    state->up = vec3New(0.0f, 0.0f, 0.0f);
    state->right = vec3New(0.0f, 0.0f, 0.0f);
    state->forward = vec3New(0.0f, 0.0f, 0.0f);

    state->accel = 0.0f;

    state->position = vec3New(0.0f, 0.0f, 0.0f);
    state->velocity = vec3New(0.0f, 0.0f, 0.0f);
    state->targetVelocity = vec3New(0.0f, 0.0f, 0.0f);

    state->angles = vec3New(0.0f, 0.0f, 0.0f);
    state->targetAngles = vec3New(0.0f, 0.0f, 0.0f);
    state->angleVelocity = vec3New(0.0f, 0.0f, 0.0f);

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

void freeState(State *state) {

    free(state->up);
    free(state->right);
    free(state->forward);

    free(state->position);
    free(state->velocity);
    free(state->targetVelocity);

    free(state->angles);
    free(state->targetAngles);
    free(state->angleVelocity);

    free(state);
}
