#include "MatrixMath.h"

vec3 vec3New(float x, float y, float z) {
    vec3 vector = (vec3) malloc(sizeof(float) * 3);
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
    return vector;
}

vec4 vec4New(float x, float y, float z, float w) {
    vec4 vector = (vec4) malloc(sizeof(float) * 4);
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
    vector[3] = w;
    return vector;
}

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 * # # # # # # # # # # #  Matrix # # # # # # # # # # # # #
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # */

mat4 mat4New() {

    mat4 matrix = (mat4) malloc(sizeof (float) * 16);

    return matrix;
}

void add(mat4 A, mat4 B, mat4 target) {

    for (int i = 0; i < 16; ++i) {
        target[i] = A[i] + B[i];
    }
}

void sub(mat4 A, mat4 B, mat4 target) {

    for (int i = 0; i < 16; ++i) {
        target[i] = A[i] - B[i];
    }
}

void mult(mat4 A, mat4 B, mat4 target) {

    /* iterates over the columns of B */
    for (int i = 0; i < 4; ++i) {

        /* sets one row of AB (transposing to column-major) */
        target[4 * i + 0] = A[0] * B[4 * i] + A[4] * B[4 * i + 1] + A[8] * B[4 * i + 2] + A[12] * B[4 * i + 3];
        target[4 * i + 1] = A[1] * B[4 * i] + A[5] * B[4 * i + 1] + A[9] * B[4 * i + 2] + A[13] * B[4 * i + 3];
        target[4 * i + 2] = A[2] * B[4 * i] + A[6] * B[4 * i + 1] + A[10] * B[4 * i + 2] + A[14] * B[4 * i + 3];
        target[4 * i + 3] = A[3] * B[4 * i] + A[7] * B[4 * i + 1] + A[11] * B[4 * i + 2] + A[15] * B[4 * i + 3];
    }
}

void multMatVec(mat4 A, vec4 v, vec4 target) {

    target[0] = A[0] * v[0] + A[4] * v[1] + A[8] * v[2] + A[12] * v[3];
    target[1] = A[1] * v[0] + A[5] * v[1] + A[9] * v[2] + A[13] * v[3];
    target[2] = A[2] * v[0] + A[6] * v[1] + A[10] * v[2] + A[14] * v[3];
    target[3] = A[3] * v[0] + A[7] * v[1] + A[11] * v[2] + A[15] * v[3];
}

void scale(mat4 target, float x, float y, float z) {

    float data[] = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };

    _setData(target, data);
}

void translate(mat4 target, float x, float y, float z) {

    float data[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };

    _setData(target, data);
}

void perspective(mat4 target, float near, float far, float fov, float ratio) {

    float z_d = far - near;

    float e00 = 1 / tanf(RAD(fov / 2.0f));
    float e11 = ratio * e00;
    float e22 = -1.0f * (far + near) / z_d;
    float e32 = -2.0f * near * far / z_d;

    /* already transposed to column-major */
    float data[] = {
        e00,   0,   0,     0,
        0, e11,   0,     0,
        0,   0, e22, -1.0f,
        0,   0, e32,     0
    };

    _setData(target, data);
}

void perspectiveInf(mat4 target, float near, float fov, float ratio) {

    /* here, e22 and e32 are derived from taking the
     * limit of original e22 and e32 to infinity */

    float e00 = 1 / tanf(RAD(fov / 2.0f));
    float e11 = ratio * e00;
    float e22 = -1.0f;
    float e32 = -2.0f * near;

    /* already transposed to column-major */
    float data[] = {
        e00,   0,   0,     0,
        0, e11,   0,     0,
        0,   0, e22, -1.0f,
        0,   0, e32,     0
    };

    _setData(target, data);
}

void rotate(mat4 target, float angle, float x, float y, float z) {

    float q[4];
    setQuat(q, angle, x, y, z);

    rotateQ(target, q);
}

void rotateQ(mat4 target, quat q) {

    normQ(q);

    float xx = q[1] * q[1];
    float yy = q[2] * q[2];
    float zz = q[3] * q[3];

    float xy = q[1] * q[2];
    float xz = q[1] * q[3];
    float wx = q[0] * q[1];
    float wy = q[0] * q[2];
    float wz = q[0] * q[3];
    float yz = q[2] * q[3];

    /* again, transposed */
    float data[] = {
        1 - 2.0f * (yy + zz),   2.0f * (xy + wz),   2.0f * (xz - wy), 0,
        2.0f * (xy - wz), 1 - 2.0f * (xx + zz),   2.0f * (yz + wx), 0,
        2.0f * (xz + wy),   2.0f * (yz - wx), 1 - 2.0f * (xx + yy), 0,
        0,         0,         0, 1
    };

    _setData(target, data);
}

void rotate3D(mat4 target, vec3 angles) {

    float quatX[4], quatY[4], quatZ[4];

    // make quat for every axis
    setQuat(quatY, angles[1], 1, 0, 0);
    setQuat(quatX, angles[0], 0, 1, 0);
    setQuat(quatZ, angles[2], 0, 0, -1);

    // X * Y
    float quatXY[4];
    multQ(quatX, quatY, quatXY);

    // XY * Z
    float quatXYZ[4];
    multQ(quatXY, quatZ, quatXYZ);

    // convert orientation quat to matrix
    rotateQ(target, quatXYZ);
}


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 * # # # # # # # # # # # Quaternions # # # # # # # # # # #
 * # # # # # # # # # # # # # # # # # # # # # # # # # # # # */

quat quatNew() {

    float *versor = (float *) malloc(sizeof(float) * 4);

    return versor;
}

void setQuat(quat target, float angle, float x, float y, float z) {

    float teta = RAD(angle / 2.0f);

    target[0] = cosf(teta);
    target[1] = sinf(teta) * x;
    target[2] = sinf(teta) * y;
    target[3] = sinf(teta) * z;
}


void multQ(quat q, quat r, quat target) {

    target[0] = r[0] * q[0] - r[1] * q[1] - r[2] * q[2] - r[3] * q[3];
    target[1] = r[0] * q[1] + r[1] * q[0] - r[2] * q[3] + r[3] * q[2];
    target[2] = r[0] * q[2] + r[1] * q[3] + r[2] * q[0] - r[3] * q[1];
    target[3] = r[0] * q[3] - r[1] * q[2] + r[2] * q[1] + r[3] * q[0];
}

void normQ(quat q) {

    float norm = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

    /* return if of unit length */
    if ( (norm - 1.0f) < 0.00001f ) return;

    norm = sqrtf(norm);

    q[0] /= norm;
    q[1] /= norm;
    q[2] /= norm;
    q[3] /= norm;
}

void invertQ(quat q) {

    /* invert axis only */
    q[1] *= -1.0f;
    q[2] *= -1.0f;
    q[3] *= -1.0f;
}

/* private helper */
void _setData(mat4 target, float *data) {

    for (int i = 0; i < 16; ++i) {
        target[i] = data[i];
    }
}

void _printM(mat4 A, const char *name) {

    printf("mat4 %s =\n", name);
    for (int i = 0; i < 4; ++i) {
        printf("| %.5f \t%.5f \t%.5f \t%.5f |\n", A[i], A[4 + i], A[8 + i], A[12 + i]);
    }
    printf("\n");
}

void _printQ(quat q, const char *name) {
    printf("\nquat %s = (%.5f,\t%.5f,\t%.5f,\t%.5f)\n", name, q[0], q[1], q[2], q[3]);
}