#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define PI (3.141592)
#define RAD(x) (x*PI/180)

#define printMat(m) (_printM(m, #m))
#define printQuat(q) (_printQ(q, #q))

#define quatToMat(quat, mat) (rotateQ(mat, quat))

typedef float *vec3;
typedef float *vec4;
typedef float *quat;
typedef float *mat4;

/* [ vector ] */

vec3 vec3New(float x, float y, float z);
vec4 vec4New(float x, float y, float z, float w);

/* [ matrix ] */

mat4 mat4New();

void add(mat4 A, mat4 B, mat4 target);
void sub(mat4 A, mat4 B, mat4 target);
void mult(mat4 A, mat4 B, mat4 target);
void multMatVec(mat4 A, vec4 v, vec4 target);

void scale(mat4 target, float x, float y, float z);
void translate(mat4 target, float x, float y, float z);
void perspective(mat4 target, float near, float far, float fovx, float ratio);
void perspectiveInf(mat4 target, float near, float fov, float ratio);
void rotate(mat4 target, float angle, float x, float y, float z);
void rotateQ(mat4 target, quat q);

/* [ quaternion ] */

quat quatNew();

void setQuat(quat q, float angle, float x, float y, float z);
void multQ(quat q, quat r, quat target);
void normQ(quat q);
void invertQ(quat q);

/* private helper */
void _setData(mat4 target, float *data);
void _printM(mat4 A, const char *name);
void _printQ(quat q, const char *name);

#endif  /* MATRIXMATH_H */

