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

    float idOri[] = { 1.0f, 0.0f, 0.0f, 0.0f };
    cpyBuf( state->orientation, idOri, 4 );
    cpyBuf( state->targetOrien, idOri, 4 );
    setBuf( state->torque, 3, 0.0f );

    return state;
}

/* physics callback */
extern float force[3], torque[3];
void applyForceAndTorque( const NewtonBody* body, float ts, int tid ) {

    float mass, ixx, iyy, izz;
    NewtonBodyGetMassMatrix( body, &mass, &ixx, &iyy, &izz );
    
    float rota[4];
    NewtonBodyGetRotation( body, rota );
    quatInv( rota );
    quatNorm( rota );
    float rotaMat[16];
    quatToMat( rotaMat, rota );

    float pos[3];
    NewtonBodyGetPosition( body, pos );
    float transMat[16];
    mat4trans( transMat, -pos[0], -pos[1], -pos[2] );

    float transform[16];
    mat4mult( rotaMat, transMat, transform );
    printBuf( transform, 16, 4, 4 );
    float F[3] = { force[0], force[1] + ( -4.9f * mass ), force[2] };
    mat4multVec3( transform, F, F );
    printBuf( F, 3, 1, 3 );

    NewtonBodyAddTorque( body, torque );
    NewtonBodyAddForce( body, F );
}

void onTransform( const NewtonBody* body, const float* const mat, int tid ) {

    State *state = NewtonBodyGetUserData( body );

    NewtonBodyGetPosition( body, state->position );
    NewtonBodyGetRotation( body, state->orientation );
}

void applyTorque( State *state ) {

    float qx[4];
    quatSet( qx, state->torque[0], state->up[0], state->up[1], state->up[2] );
    float qy[4];
    quatSet( qy, state->torque[1], state->right[0], state->right[1], state->right[2] );
    float qz[4];
    quatSet( qz, state->torque[2], state->forward[0], state->forward[1], state->forward[2] );

    float qxy[4];
    quatMult( qx, qy, qxy );
    float qxyz[4];
    quatMult( qxy, qz, qxyz );
    quatNorm( qxyz );

    /* consume torque */
    setBuf( state->torque, 3, 0.0f );

    quatMult( qxyz, state->targetOrien, state->targetOrien );
}

void calcOrientation( quat target, State *state, float alpha) {
    
    quatSlerp( state->orientation, state->targetOrien, target, alpha );
    quatNorm( target );
}

void calcPosition( vec3 target, State *state, float alpha) {

    float temp[3];
    temp[0] = state->position[0] + (state->forward[0] * state->velocity[2] + state->right[0] * state->velocity[0] + state->up[0] * state->velocity[1]) * alpha;
    temp[1] = state->position[1] + (state->forward[1] * state->velocity[2] + state->right[1] * state->velocity[0] + state->up[1] * state->velocity[1]) * alpha;
    temp[2] = state->position[2] + (state->forward[2] * state->velocity[2] + state->right[2] * state->velocity[0] + state->up[2] * state->velocity[1]) * alpha;
    cpyBuf( target, temp, 3 );
}


