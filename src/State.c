#include "Util.h"
#include "State.h"
#include "common.h"
#include "MatrixMath.h"

State *newState() {

    State *state = alloc( State, 1 );

    state->accel = 0.0f;

    setBuf( state->up, 3, 0.0f );
    setBuf( state->right, 3, 0.0f );
    setBuf( state->forward, 3, 0.0f );

    setBuf( state->position, 3, 0.0f );
    setBuf( state->velocity, 3, 0.0f );
    setBuf( state->targetVelocity, 3, 0.0f );

    setBuf( state->angles, 3, 0.0f );
    setBuf( state->targetAngles, 3, 0.0f );
    setBuf( state->angleVelocity, 3, 0.0f );

    return state;
}

void calcAngles( vec3 target, State* state, float alpha ) {

    target[0] = state->angles[0] + state->angleVelocity[0]*alpha;
    target[1] = state->angles[1] + state->angleVelocity[1]*alpha;
    target[2] = state->angles[2] + state->angleVelocity[2]*alpha;
}

void calcOrientation( quat target, State *state, float alpha) {
    
    float qx[4];
    quatSet( qx, state->angleVelocity[0]*alpha, state->up[0], state->up[1], state->up[2] );
    float qy[4];
    quatSet( qy, state->angleVelocity[1]*alpha, state->right[0], state->right[1], state->right[2] );
    float qz[4];
    quatSet( qz, state->angleVelocity[2]*alpha, state->forward[0], state->forward[1], state->forward[2] );

    float qxy[4];
    quatMult( qx, qy, qxy );
    float qxyz[4];
    quatMult( qxy, qz, qxyz );
    quatNorm( qxyz );

    float orientation[4];
    quatMult( qxyz, state->orientation, orientation );
    cpyBuf( target, orientation, 4 );
}

void calcPosition( vec3 target, State *state, float alpha) {

    target[0] = state->position[0] + (state->forward[0] * state->velocity[2] + state->right[0] * state->velocity[0] + state->up[0] * state->velocity[1]) * alpha;
    target[1] = state->position[1] + (state->forward[1] * state->velocity[2] + state->right[1] * state->velocity[0] + state->up[1] * state->velocity[1]) * alpha;
    target[2] = state->position[2] + (state->forward[2] * state->velocity[2] + state->right[2] * state->velocity[0] + state->up[2] * state->velocity[1]) * alpha;
}


