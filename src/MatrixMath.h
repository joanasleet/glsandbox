#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define PI (3.141592f)
#define RAD(x) (x * PI / 180.0f)

#define printMat(m) (_printM(m, #m))
#define printQuat(q) (_printQ(q, #q))
#define printVec3(v) (_printVec3( v, #v ))

#define quatToMat(quat, mat) (rotateQ(mat, quat))

#define nullVec( vec3 ) \
    vec3[0] = 0; \
    vec3[1] = 0; \
    vec3[2] = 0; \

#define setVec3( target, source ) \
    target[0] = source[0]; \
    target[1] = source[1]; \
    target[2] = source[2]; \

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
void vec3add( vec3 a, vec3 b, vec3 target );
void vec3sub( vec3 a, vec3 b, vec3 target );
void vec3scale( float s, vec3 target );
void vec3normalize( vec3 v );
float vec3norm( vec3 v );

/* [ matrix ] */
void add(mat4 A, mat4 B, mat4 target);
void sub(mat4 A, mat4 B, mat4 target);
void mult(mat4 A, mat4 B, mat4 target);
void multMatVec4(mat4 A, vec4 v, vec4 target);
void multMatVec3(mat4 A, vec3 v, vec3 target);

void scale(mat4 target, float x, float y, float z);
void translate(mat4 target, float x, float y, float z);

void perspective(mat4 target, float near, float far, float fovx, float ratio);
void perspectiveInf(mat4 target, float near, float fov, float ratio);

void rotate(mat4 target, float angle, float x, float y, float z);
void rotateQ(mat4 target, quat q);
void rotate3D(quat target, vec3 angles);

/* [ quaternion ] */
void setQuat(quat q, float angle, float x, float y, float z);
void multQ(quat q, quat r, quat target);
void normQ(quat q);
void invertQ(quat q);

/* private helper */
void _setData(mat4 target, float *data);
void _printM(mat4 A, const char *name);
void _printQ(quat q, const char *name);
void _printVec3(vec3 v, const char *name);

/* [ interpolation ] */
float lerpStepf(float from, float to, float alpha);
double lerpStep(double from, double to, double alpha);

#endif

