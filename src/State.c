#include "common.h"

#include "State.h"

State *newState() {

    State *state = NEW(State);

    state->position = vec3New(0.0f, 0.0f, 0.0f);
    state->velocity = vec3New(0.0f, 0.0f, 0.0f);

    state->angles = vec3New(0.0f, 0.0f, 0.0f);
    state->angleVelocity = vec3New(0.0f, 0.0f, 0.0f);

    return state;
}

void freeState(State *state) {

    free(state->position);
    free(state->velocity);
    free(state->angles);
    free(state->angleVelocity);
    free(state);
}