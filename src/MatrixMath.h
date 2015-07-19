#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define PI (3.141592f)
#define RAD(x) (x * PI / 180.0f)

#define printBuf( b, n, r, c ) ( _printBuffer( b, #b, n, r, c ) )

#define setBuf( b, n, val ) \
    for( int _i=0; _i<n; ++_i ) b[_i] = val; \

#define cpyBuf( trgt, src, n ) \
    for( int _i=0; _i<n; ++_i ) trgt[_i] = src[_i]; \

#define sphereMap( target, origin, radius ) \
    vec3sub( target, origin, target );      \
    vec3normalize( target );                \
    vec3scale( radius, target );            \
    vec3add( origin, target, target );      \

typedef float *vec3;
typedef float *vec4;
typedef float *quat;
typedef float *mat4;

/* [ vector ] */
float vec3norm( vec3 v );
float vec3dot( vec3 a, vec3 b );

void vec3normalize( vec3 v );
void vec3scale( float s, vec3 target );
void vec3add( vec3 a, vec3 b, vec3 target );
void vec3sub( vec3 a, vec3 b, vec3 target );
void vec3cross( vec3 a, vec3 b, vec3 target );

/* [ matrix ] */
void mat4add(mat4 A, mat4 B, mat4 target);
void mat4sub(mat4 A, mat4 B, mat4 target);
void mat4mult(mat4 A, mat4 B, mat4 target);
void mat4multVec4(mat4 A, vec4 v, vec4 target);
void mat4multVec3(mat4 A, vec3 v, vec3 target);

void mat4scale(mat4 target, float x, float y, float z);
void mat4trans(mat4 target, float x, float y, float z);
void mat4rotate(mat4 target, float angle, float x, float y, float z);
void mat4persp(mat4 target, float near, float far, float fovx, float ratio);
void mat4perspinf(mat4 target, float near, float fov, float ratio);

void rotate3D(quat target, vec3 angles);

/* [ quaternion ] */
void quatInv(quat q);
void quatNorm(quat q);
void quatToMat(mat4 target, quat q);
void quatMult(quat q, quat r, quat target);
void quatSet(quat q, float angle, float x, float y, float z);

/* [ interpolation ] */
float lerpStepf(float from, float to, float alpha);
double lerpStep(double from, double to, double alpha);

#endif

