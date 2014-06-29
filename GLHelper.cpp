#include "GLHelper.h"

#include <math.h>

GLfloat* cube(float length, float midX, float midY, float midZ) {
    GLfloat* cubeData = (GLfloat*) malloc(sizeof (GLfloat) * 8 * 4);

    float hlen = length / 2.0f;

    // front
    // UpL
    cubeData[0] = midX - hlen; // x
    cubeData[1] = midY + hlen; // y
    cubeData[2] = midZ + hlen; // z
    cubeData[3] = 1.0f; // w 

    // UpR
    cubeData[4] = midX + hlen; // x
    cubeData[5] = midY + hlen; // y
    cubeData[6] = midZ + hlen; // z
    cubeData[7] = 1.0f; // w 

    // BotR
    cubeData[8] = midX + hlen; // x
    cubeData[9] = midY - hlen; // y
    cubeData[10] = midZ + hlen; // z
    cubeData[11] = 1.0f; // w 

    // BotL
    cubeData[12] = midX - hlen; // x
    cubeData[13] = midY - hlen; // y
    cubeData[14] = midZ + hlen; // z
    cubeData[15] = 1.0f; // w 

    // back
    // UpL
    cubeData[16] = midX - hlen; // x
    cubeData[17] = midY + hlen; // y
    cubeData[18] = midZ - hlen; // z
    cubeData[19] = 1.0f; // w 

    // UpR
    cubeData[20] = midX + hlen; // x
    cubeData[21] = midY + hlen; // y
    cubeData[22] = midZ - hlen; // z
    cubeData[23] = 1.0f; // w 

    // BotR
    cubeData[24] = midX + hlen; // x
    cubeData[25] = midY - hlen; // y
    cubeData[26] = midZ - hlen; // z
    cubeData[27] = 1.0f; // w 

    // BotL
    cubeData[28] = midX - hlen; // x
    cubeData[29] = midY - hlen; // y
    cubeData[30] = midZ - hlen; // z
    cubeData[31] = 1.0f; // w 

    return cubeData;
}

GLuint loadTexture(const char* file, GLuint texture, GLboolean genMips) {

    return 0;
}

/**
 * creates area A = [minX, maxX] x [minY, maxY] with evenly spacing.
 * step length for n points in [a, b] is (b-a)/(n-1).
 **/
GLdouble* createArea(double minX, double maxX, double minY, double maxY, int resx, int resy) {

    GLdouble* area = (GLdouble*) malloc(sizeof (GLdouble) * 2 * resx * resy);

    GLdouble xit = minX;
    GLdouble yit = maxY;

    GLdouble xstep = (maxX - minX) / (double) (resx - 1);
    GLdouble ystep = (maxY - minY) / (double) (resy - 1);

    for (int i = 0; i < resy; i++) {

        for (int j = 0; j < 2 * resx; j += 2) {

            area[i * (2 * resy) + j] = (xit += xstep);
            area[i * (2 * resy) + j + 1] = yit;
        }

        yit -= ystep;
        xit = minX;
    }

    return area;
}

GLfloat* colorMap(float startR, float startG, float startB, float endR, float endG, float endB, int length) {
    GLfloat* map = (GLfloat*) malloc(sizeof (GLfloat) * 3 * length);

    GLfloat stepR = (endR - startR) / (length - 1);
    GLfloat stepG = (endG - startG) / (length - 1);
    GLfloat stepB = (endB - startB) / (length - 1);

    for (int i = 0; i < 3 * length; i += 3) {
        map[i] = fabs(startR);
        startR += stepR;

        map[i + 1] = fabs(startG);
        startG += stepG;

        map[i + 2] = fabs(startB);
        startB += stepB;
    }

    return map;
}
