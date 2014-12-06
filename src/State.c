#include "common.h"

#include "State.h"

State *newState() {

    State *state = NEW(State);

    state->position = vec3New(0.0f, 0.0f, 0.0f);
    state->velocity = vec3New(0.0f, 0.0f, 0.0f);
    state->orientation = quatNew();

    return state;
}

void freeState(State *state) {

    free(state->position);
    free(state->velocity);
    free(state->orientation);
    free(state);
}