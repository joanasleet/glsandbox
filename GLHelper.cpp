#include "GLHelper.h"

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
GLfloat* createArea(float minX, float maxX, float minY, float maxY, int n) {

    GLfloat* area = (GLfloat*) malloc(sizeof (GLfloat) * (2 * n) * n);

    GLfloat xit = minX;
    GLfloat yit = maxY;

    GLfloat xstep = (maxX - minX) / (float) (n - 1);
    GLfloat ystep = (maxY - minY) / (float) (n - 1);

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < 2 * n; j += 2) {

            area[i * (2 * n) + j] = (xit += xstep);
            area[i * (2 * n) + j + 1] = yit;
        }

        yit -= ystep;
        xit = minX;
    }

    return area;
}

void printArea(GLfloat* area, int size) {

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
        map[i] = (startR += stepR);
        map[i + 1] = (startG += stepG);
        map[i + 2] = (startB += stepB);
    }

    return map;
}
