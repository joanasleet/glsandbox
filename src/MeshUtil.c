#include "MeshUtil.h"
#include "Debugger.h"
#include "Deallocator.h"
#include "LookupManager.h"

#include "string.h"

#define ASCII_OFFSET 32
#define FONTMAP_SIZE 16
#define FONTMAP_SIZE_F 16.0f
#define IARATIO 0.5625f

GLuint genVao(uint32 type, GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    switch (type) {
    case PLANE:
        return planeVAO(length, texRes, midX, midY, midZ, vertcount);
    case CUBE:
        return cubeVAO(length, texRes, midX, midY, midZ, vertcount);
    case CUBEMAP:
        return cubeMapVAO(length, texRes, midX, midY, midZ, vertcount);
    case OVERLAY:
        return overlayVAO(vertcount);
    case TERRAIN:
        return terrainVAO(length, midX, midY, midZ, vertcount);
    case SPHERE:
        return sphereVAO(length, texRes, midX, midY, midZ, vertcount);
    case SKYDOME:
        return sphereVAO(length, texRes, midX, midY, midZ, vertcount);
    case SCREEN_QUAD:
        return screenQuadVAO(vertcount);
    default:
        err("Unknown vao type: %d", type);
        return 0;
    }
}

GLuint planeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    *vertcount = 4;

    float l = length / 2.0f;

    GLfloat vboData[] = {
        // vertices
        midX - l, midY, midZ + l, 1.0f,
        midX + l, midY, midZ + l, 1.0f,
        midX + l, midY, midZ - l, 1.0f,
        midX - l, midY, midZ - l, 1.0f,

        // texture coords
        0.0f, 0.0f,
        texRes, 0.0f,
        texRes, texRes,
        0.0f, texRes,

        // normals
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 16));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * (16 + 8)));
    glEnableVertexAttribArray(2);

    return vao;
}

GLuint cubeMapVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    float l = length / 2.0f;

    *vertcount = 24;

    GLfloat vboData[] = {
        // vertices
        // bot
        midX - l, midY - l, midZ + l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX - l, midY - l, midZ - l, 1.0f,

        // top
        midX - l, midY + l, midZ + l, 1.0f,
        midX - l, midY + l, midZ - l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX + l, midY + l, midZ + l, 1.0f,

        // front
        midX - l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ + l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
        midX - l, midY - l, midZ + l, 1.0f,

        // back
        midX - l, midY - l, midZ - l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX - l, midY + l, midZ - l, 1.0f,

        // left
        midX - l, midY - l, midZ + l, 1.0f,
        midX - l, midY - l, midZ - l, 1.0f,
        midX - l, midY + l, midZ - l, 1.0f,
        midX - l, midY + l, midZ + l, 1.0f,

        // right
        midX + l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    return vao;
}

GLuint sphereVAO(GLfloat radius, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    GLuint vao = cubeVAO(radius, texRes, midX, midY, midZ, vertcount);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    return vao;
}


GLuint circleVAO(GLfloat radius, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    return sphereVAO(radius, texRes, midX, midY, midZ, vertcount);

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    radius = 10;

    int n = 1000;

    int vertices = n * 2 + 1;
    *vertcount = vertices;

    int size = 4 * vertices;
    int hsize = 2 * (2 * n);
    float data[size];

    float y = 0.0f;
    float x = -radius;
    float xstep = (2.0f * radius) / (n - 1);

    // triangle fan ref point
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 1.0f;

    for (int i = 4; i <= hsize; i += 4) {

        y = sqrtf( abs(radius * radius - x * x) );
        //fprintf(stderr, "[x = %f]\t[y = %f]\n", x, y);

        // upper sphere
        data[i] = x;
        data[i + 1] = y;
        data[i + 2] = 0.0f;
        data[i + 3] = 1.0f;

        // lower sphere
        data[size - i] = x;
        data[size - i + 1] = -y;
        data[size - i + 2] = 0.0f;
        data[size - i + 3] = 1.0f;

        x += xstep;
    }


    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    //glEnableVertexAttribArray(1);

    return vao;
}

GLuint cubeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    float l = length / 2.0f;

    *vertcount = 24;

    GLfloat vboData[] = {
        // vertices
        // bot
        midX - l, midY - l, midZ - l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
        midX - l, midY - l, midZ + l, 1.0f,

        // top
        midX + l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX - l, midY + l, midZ - l, 1.0f,
        midX - l, midY + l, midZ + l, 1.0f,

        // front
        midX - l, midY - l, midZ + l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
        midX + l, midY + l, midZ + l, 1.0f,
        midX - l, midY + l, midZ + l, 1.0f,

        // back
        midX - l, midY + l, midZ - l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX - l, midY - l, midZ - l, 1.0f,

        // left
        midX - l, midY + l, midZ + l, 1.0f,
        midX - l, midY + l, midZ - l, 1.0f,
        midX - l, midY - l, midZ - l, 1.0f,
        midX - l, midY - l, midZ + l, 1.0f,

        // right
        midX + l, midY - l, midZ + l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX + l, midY + l, midZ + l, 1.0f,

        // texture coords
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // normals
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 96));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * (96 + 48)));
    glEnableVertexAttribArray(2);

    return vao;
}

// TODO: Some Debugging
// - winding order
// - test influence of parameters
GLuint terrainVAO(GLfloat length, GLfloat midX, GLfloat midY, GLfloat midZ, int32 *vertcount) {

    const int patch_dim = 10;
    const int patch_verts = 4;

    const float plen = length / (float) patch_dim;
    const float hlen = length / 2.0f;

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    *vertcount = patch_dim * patch_dim * patch_verts;

    GLfloat data[ (*vertcount) * 4];

    for (int i = 0; i < patch_dim; ++i) {

        for (int j = 0; j < patch_dim; ++j) {

            data[16 * (patch_dim * i + j) + 0] = midX - hlen + j * (plen);
            data[16 * (patch_dim * i + j) + 1] = midY;
            data[16 * (patch_dim * i + j) + 2] = midZ - hlen + i * (plen);
            data[16 * (patch_dim * i + j) + 3] = 1.0f;

            data[16 * (patch_dim * i + j) + 4] = midX - hlen + j * (plen) + plen;
            data[16 * (patch_dim * i + j) + 5] = midY;
            data[16 * (patch_dim * i + j) + 6] = midZ - hlen + i * (plen);
            data[16 * (patch_dim * i + j) + 7] = 1.0f;

            data[16 * (patch_dim * i + j) + 8] = midX - hlen + j * (plen) + plen;
            data[16 * (patch_dim * i + j) + 9] = midY;
            data[16 * (patch_dim * i + j) + 10] = midZ - hlen + i * (plen) + plen;
            data[16 * (patch_dim * i + j) + 11] = 1.0f;

            data[16 * (patch_dim * i + j) + 12] = midX - hlen + j * (plen);
            data[16 * (patch_dim * i + j) + 13] = midY;
            data[16 * (patch_dim * i + j) + 14] = midZ - hlen + i * (plen) + plen;
            data[16 * (patch_dim * i + j) + 15] = 1.0f;
        }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, patch_verts);

    return vao;
}

GLuint overlayVAO(int32 *vertcount) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    *vertcount = 4;

    const GLfloat data[] = {

        -1.0f, -1.0f, 0.0, 1.0f,
        -1.0f, 1.0f, 0.0, 1.0f,
        1.0f, 1.0f, 0.0, 1.0f,
        1.0f, -1.0f, 0.0, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 16));
    glEnableVertexAttribArray(1);

    return vao;
}

GLuint staticTextVAO(const char *text, GLfloat size, GLfloat x, GLfloat y, int32 *vertcount) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    int strl = strlen(text);

    // font size scale factor
    size /= 150.0f; // to be determined

    // vertex per char
    uint32 vertices = 4 * strl;
    *vertcount = vertices;

    // tex + position coords
    vertices *= (2 + 4);
    GLfloat data[vertices];

    GLfloat xstep = size * IARATIO;

    char c, indx;
    GLfloat texstep;
    GLfloat xleft, xright, ybot /*, ytop */;
    GLfloat textopleftX, textopleftY;

    info("Generating %d quads for text '%s'", strl, text);
    for (int i = 0; i < strl; ++i) {

        xleft = x + i * xstep;
        xright = x + (i + 1) * xstep;

        /* ytop = y */
        ybot = y - size;

        // top left
        data[16 * i] = xleft;
        data[16 * i + 1] = y;
        data[16 * i + 2] = 0.0f;
        data[16 * i + 3] = 1.0f;

        // top right
        data[16 * i + 4] = xright;
        data[16 * i + 5] = y;
        data[16 * i + 6] = 0.0f;
        data[16 * i + 7] = 1.0f;

        // bot right
        data[16 * i + 8] = xright;
        data[16 * i + 9] = ybot;
        data[16 * i + 10] = 0.0f;
        data[16 * i + 11] = 1.0f;

        // bot left
        data[16 * i + 12] = xleft;
        data[16 * i + 13] = ybot;
        data[16 * i + 14] = 0.0f;
        data[16 * i + 15] = 1.0f;

        printf("#%d quad tex coords\n", i);

        c = text[i];
        indx = c - ASCII_OFFSET;

        printf("(char, index): (%c, %d)\n", c, indx);

        texstep = (indx % FONTMAP_SIZE) / FONTMAP_SIZE_F;
        textopleftX = texstep;
        textopleftY = 1.0f - (indx / FONTMAP_SIZE) / FONTMAP_SIZE_F;

        // tex topleft
        data[16 * strl + 8 * i] = textopleftX;
        data[16 * strl + 8 * i + 1] = textopleftY;

        // tex topright
        data[16 * strl + 8 * i + 2] = textopleftX + 0.0625f;
        data[16 * strl + 8 * i + 3] = textopleftY;

        // tex botright
        data[16 * strl + 8 * i + 4] = textopleftX + 0.0625f;
        data[16 * strl + 8 * i + 5] = textopleftY - 0.0625f;

        // tex botleft
        data[16 * strl + 8 * i + 6] = textopleftX;
        data[16 * strl + 8 * i + 7] = textopleftY - 0.0625f;

        printf("- - -\n");
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(GLfloat) * 16 * strl));
    glEnableVertexAttribArray(1);

    return vao;
}

GLuint screenQuadVAO(int32 *vertcount) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    *vertcount = 4;

    const GLfloat data[] = {

        -1.0f, -1.0f, 0.0, 1.0f,
        1.0f, -1.0f, 0.0, 1.0f,
        1.0f, 1.0f, 0.0, 1.0f,
        -1.0f, 1.0f, 0.0, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    return vao;

}
