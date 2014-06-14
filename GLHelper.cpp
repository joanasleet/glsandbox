#include "GLHelper.h"

#include <math.h>

GLuint genId(enum GEN_TYPE type) {

    GLuint* ids = genIds(type, 1);
    GLuint id = ids[0];
    free(ids);

    return id;
}

GLuint* genIds(enum GEN_TYPE type, int count) {

    GLuint* ids = (GLuint*) malloc(sizeof (GLuint) * count);

    switch (type) {
        case VertexArray:
            glGenVertexArrays(count, ids);
            break;
        case Buffer:
            glGenBuffers(count, ids);
            break;
    }

    return ids;
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

void printArea(GLdouble* area, int size) {

    for (int i = 0; i < size; i++) {

        for (int j = 0; j < 2 * size; j += 2) {

            printf("(%.3f, %.3f) ", area[i * (2 * size) + j], area[i * (2 * size) + j + 1]);
        }

        printf("\n");
    }
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
