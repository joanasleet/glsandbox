#include "MatrixMath.h"

/*
 * vector stuff */

void vec3add( vec3 a, vec3 b, vec3 target ) {

    target[0] = a[0] + b[0];
    target[1] = a[1] + b[1];
    target[2] = a[2] + b[2];
}

void vec3sub( vec3 a, vec3 b, vec3 target ) {

    target[0] = a[0] - b[0];
    target[1] = a[1] - b[1];
    target[2] = a[2] - b[2];
}

void vec3scale( float s, vec3 target ) {

    target[0] *= s;
    target[1] *= s;
    target[2] *= s;
}

float vec3norm( vec3 v ) {

    return sqrtf( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
}

void vec3normalize( vec3 v ) {

    float norm = vec3norm( v );

    v[0] /= norm;
    v[1] /= norm;
    v[2] /= norm;
}

float vec3dot( vec3 a, vec3 b ) {
    return ( a[0]*b[0] + a[1]*b[1] + a[2]*b[2] );
}

void vec3cross( vec3 a, vec3 b, vec3 target ) {

    target[0] = a[1]*b[2] - a[2]*b[1];
    target[1] = a[2]*b[0] - a[0]*b[2];
    target[2] = a[0]*b[1] - a[1]*b[0];
}

/*
 * matrix stuff */

void mat4add(mat4 A, mat4 B, mat4 target) {

    for (int i = 0; i < 16; ++i) {
        target[i] = A[i] + B[i];
    }
}

void mat4sub(mat4 A, mat4 B, mat4 target) {

    for (int i = 0; i < 16; ++i) {
        target[i] = A[i] - B[i];
    }
}

void mat4mult(mat4 A, mat4 B, mat4 target) {

    /* iterates over the columns of B */
    for (int i = 0; i < 4; ++i) {

        /* sets one row of AB (transposing to column-major) */
        target[4 * i + 0] = A[0] * B[4 * i] + A[4] * B[4 * i + 1] + A[8] * B[4 * i + 2] + A[12] * B[4 * i + 3];
        target[4 * i + 1] = A[1] * B[4 * i] + A[5] * B[4 * i + 1] + A[9] * B[4 * i + 2] + A[13] * B[4 * i + 3];
        target[4 * i + 2] = A[2] * B[4 * i] + A[6] * B[4 * i + 1] + A[10] * B[4 * i + 2] + A[14] * B[4 * i + 3];
        target[4 * i + 3] = A[3] * B[4 * i] + A[7] * B[4 * i + 1] + A[11] * B[4 * i + 2] + A[15] * B[4 * i + 3];
    }
}

void mat4multVec4(mat4 A, vec4 v, vec4 target) {

    target[0] = A[0] * v[0] + A[4] * v[1] + A[8] * v[2] + A[12] * v[3];
    target[1] = A[1] * v[0] + A[5] * v[1] + A[9] * v[2] + A[13] * v[3];
    target[2] = A[2] * v[0] + A[6] * v[1] + A[10] * v[2] + A[14] * v[3];
    target[3] = A[3] * v[0] + A[7] * v[1] + A[11] * v[2] + A[15] * v[3];
}

void mat4multVec3(mat4 A, vec3 v, vec3 target) {

    target[0] = A[0] * v[0] + A[4] * v[1] +  A[8] * v[2] + A[12];
    target[1] = A[1] * v[0] + A[5] * v[1] +  A[9] * v[2] + A[13];
    target[2] = A[2] * v[0] + A[6] * v[1] + A[10] * v[2] + A[14];
}

void mat4scale(mat4 target, float x, float y, float z) {

    float data[] = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };

    cpyBuf( target, data, 16 );
}

void mat4trans(mat4 target, float x, float y, float z) {

    float data[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };

    cpyBuf( target, data, 16 );
}

void mat4persp(mat4 target, float near, float far, float fov, float ratio) {

    float z_d = far - near;

    float e00 = 1 / tanf(RAD(fov / 2.0f));
    float e11 = ratio * e00;
    float e22 = -1.0f * (far + near) / z_d;
    float e32 = -2.0f * near * far / z_d;

    /* transposed to column-major */
    float data[] = {
        e00, 0,   0,     0,
        0, e11,   0,     0,
        0,   0, e22, -1.0f,
        0,   0, e32,     0
    };

    cpyBuf( target, data, 16 );
}

void mat4perspinf(mat4 target, float near, float fov, float ratio) {

    /* here, e22 and e32 are derived from taking the
     * limit of original e22 and e32 to infinity */

    float e00 = 1 / tanf(RAD(fov / 2.0f));
    float e11 = ratio * e00;
    float e22 = -1.0f;
    float e32 = -2.0f * near;

    /* transposed to column-major */
    float data[] = {
        e00, 0,   0,     0,
        0, e11,   0,     0,
        0,   0, e22, -1.0f,
        0,   0, e32,     0
    };

    cpyBuf( target, data, 16 );
}

void mat4rotate(mat4 target, float angle, float x, float y, float z) {

    float q[4];
    quatSet( q, angle, x, y, z );

    quatToMat( target, q );
}

void rotate3D(quat target, vec3 angles) {

    float quatX[4], quatY[4], quatZ[4];

    // make quat for every axis
    quatSet(quatY, angles[1], 1, 0, 0);
    quatSet(quatX, angles[0], 0, 1, 0);
    quatSet(quatZ, angles[2], 0, 0, -1);

    // X * Y
    float quatXY[4];
    quatMult(quatX, quatY, quatXY);

    // XY * Z
    float quatXYZ[4];
    quatMult(quatXY, quatZ, quatXYZ);

    // write to target quat
    cpyBuf( target, quatXYZ, 4 );
}

/*
 * quaternion stuff */

void quatToMat(mat4 target, quat q) {

    quatNorm(q);

    float xx = q[1] * q[1];
    float yy = q[2] * q[2];
    float zz = q[3] * q[3];

    float xy = q[1] * q[2];
    float xz = q[1] * q[3];
    float wx = q[0] * q[1];
    float wy = q[0] * q[2];
    float wz = q[0] * q[3];
    float yz = q[2] * q[3];

    /* transposed to column-major */
    float data[] = {
        1 - 2.0f * (yy + zz),     2.0f * (xy + wz),     2.0f * (xz - wy), 0,
            2.0f * (xy - wz), 1 - 2.0f * (xx + zz),     2.0f * (yz + wx), 0,
            2.0f * (xz + wy),     2.0f * (yz - wx), 1 - 2.0f * (xx + yy), 0,
                           0,                    0,                    0, 1
    };

    cpyBuf( target, data, 16 );
}

void quatSet(quat target, float angle, float x, float y, float z) {

    float teta = RAD(angle / 2.0f);

    target[0] = cosf(teta);
    target[1] = sinf(teta) * x;
    target[2] = sinf(teta) * y;
    target[3] = sinf(teta) * z;
}

void quatMult(quat q, quat r, quat target) {

    target[0] = r[0] * q[0] - r[1] * q[1] - r[2] * q[2] - r[3] * q[3];
    target[1] = r[0] * q[1] + r[1] * q[0] - r[2] * q[3] + r[3] * q[2];
    target[2] = r[0] * q[2] + r[1] * q[3] + r[2] * q[0] - r[3] * q[1];
    target[3] = r[0] * q[3] - r[1] * q[2] + r[2] * q[1] + r[3] * q[0];
}

void quatNorm(quat q) {

    float norm = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

    /* return if unit length */
    if ( (norm - 1.0f) < 0.00001f ) return;

    norm = sqrtf(norm);

    q[0] /= norm;
    q[1] /= norm;
    q[2] /= norm;
    q[3] /= norm;
}

void quatInv(quat q) {

    /* invert axis only */
    q[1] *= -1.0f;
    q[2] *= -1.0f;
    q[3] *= -1.0f;
}

/* private helper */
void _printBuffer( float* buff, const char* name, int n, int rows, int cols ) {

    printf( "%s =%c", name, ( rows <= 1 ) ? ' ' : '\n' );

    for( int r=0; r<rows; ++r ) {
        for( int c=0; c<cols; ++c ) {
            
            if( n-- == 0 ) {
                
                printf( "\n" );
                return;
            }

            printf( "%.1f\t, ", buff[rows*r+c] );
        }
        printf( "\n" );
    }
}

float lerpStepf(float from, float to, float alpha) {

    float delta = to - from;

    if (delta < 0.0000001f && delta > -0.0000001f)
        return 0.0f;

    return (delta * alpha);
}

double lerpStep(double from, double to, double alpha) {

    double delta = to - from;

    if (delta < 0.0000001 && delta > -0.0000001)
        return 0.0;

    return (delta * alpha);
}

