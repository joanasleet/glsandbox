#include "GLHelper.h"

GLfloat* cubeVD(GLsizei* size, float length, float midX, float midY, float midZ, bool genTexels) {

    GLfloat* vertexData;
    GLsizei bufferSize;

    if (genTexels) {
        bufferSize = sizeof (GLfloat)*((8 * 4) + (8 * 3));
    } else {
        bufferSize = sizeof (GLfloat)*(8 * 4);
    }

    *size = bufferSize;
    vertexData = (GLfloat*) malloc(bufferSize);

    if (!vertexData) {
        ERR("Failed to allocate vertex buffer.");
        return NULL;
    }

    length /= 2.0f;

    // front
    // UpL
    vertexData[0] = midX - length; // x
    vertexData[1] = midY + length; // y
    vertexData[2] = midZ + length; // z
    vertexData[3] = 1.0f; // w 

    // UpR
    vertexData[4] = midX + length; // x
    vertexData[5] = midY + length; // y
    vertexData[6] = midZ + length; // z
    vertexData[7] = 1.0f; // w 

    // BotR
    vertexData[8] = midX + length; // x
    vertexData[9] = midY - length; // y
    vertexData[10] = midZ + length; // z
    vertexData[11] = 1.0f; // w 

    // BotL
    vertexData[12] = midX - length; // x
    vertexData[13] = midY - length; // y
    vertexData[14] = midZ + length; // z
    vertexData[15] = 1.0f; // w 

    // back
    // UpL
    vertexData[16] = midX - length; // x
    vertexData[17] = midY + length; // y
    vertexData[18] = midZ - length; // z
    vertexData[19] = 1.0f; // w 

    // UpR
    vertexData[20] = midX + length; // x
    vertexData[21] = midY + length; // y
    vertexData[22] = midZ - length; // z
    vertexData[23] = 1.0f; // w 

    // BotR
    vertexData[24] = midX + length; // x
    vertexData[25] = midY - length; // y
    vertexData[26] = midZ - length; // z
    vertexData[27] = 1.0f; // w 

    // BotL
    vertexData[28] = midX - length; // x
    vertexData[29] = midY - length; // y
    vertexData[30] = midZ - length; // z
    vertexData[31] = 1.0f; // w 

    if (genTexels) {
        vertexData[32] = 0.0f;
        vertexData[33] = 1.0f;
        vertexData[34] = 0.0f;

        vertexData[35] = 1.0f;
        vertexData[36] = 1.0f;
        vertexData[37] = 0.0f;

        vertexData[38] = 1.0f;
        vertexData[39] = 0.0f;
        vertexData[40] = 0.0f;

        vertexData[41] = 0.0f;
        vertexData[42] = 0.0f;
        vertexData[43] = 0.0f;


        vertexData[44] = 0.0f;
        vertexData[45] = 1.0f;
        vertexData[46] = 1.0f;

        vertexData[47] = 1.0f;
        vertexData[48] = 1.0f;
        vertexData[49] = 1.0f;

        vertexData[50] = 1.0f;
        vertexData[51] = 0.0f;
        vertexData[52] = 1.0f;

        vertexData[53] = 0.0f;
        vertexData[54] = 0.0f;
        vertexData[55] = 1.0f;
    }

    return vertexData;
}

GLfloat* planeVD(GLsizei* size, GLfloat length, GLfloat midX, GLfloat midY, GLfloat midZ, bool genTexels) {

    GLfloat* vertexData;
    GLsizei bufferSize;

    if (genTexels) {
        bufferSize = sizeof (GLfloat)*((4 * 4) + (4 * 3));
    } else {
        bufferSize = sizeof (GLfloat)*(4 * 4);
    }

    *size = bufferSize;
    vertexData = (GLfloat*) malloc(bufferSize);

    if (!vertexData) {
        ERR("Failed to allocate vertex buffer.");
        return NULL;
    }

    length /= 2.0f;

    // --------
    // -      -
    // -      -
    // -------O
    vertexData[0] = midX + length;
    vertexData[1] = midY;
    vertexData[2] = midZ + length;
    vertexData[3] = 1.0f;

    // -------O
    // -      -
    // -      -
    // --------
    vertexData[4] = midX + length;
    vertexData[5] = midY;
    vertexData[6] = midZ - length;
    vertexData[7] = 1.0f;

    // O-------
    // -      -
    // -      -
    // --------
    vertexData[8] = midX - length;
    vertexData[9] = midY;
    vertexData[10] = midZ - length;
    vertexData[11] = 1.0f;

    // --------
    // -      -
    // -      -
    // O-------
    vertexData[12] = midX - length;
    vertexData[13] = midY;
    vertexData[14] = midZ + length;
    vertexData[15] = 1.0f;

    if (genTexels) {
        vertexData[16] = 10.0f;
        vertexData[17] = 0.0f;
        vertexData[18] = 0.0f;

        vertexData[19] = 10.0f;
        vertexData[20] = 10.0f;
        vertexData[21] = 0.0f;

        vertexData[22] = 0.0f;
        vertexData[23] = 10.0f;
        vertexData[24] = 0.0f;

        vertexData[25] = 0.0f;
        vertexData[26] = 0.0f;
        vertexData[27] = 0.0f;
    }

    return vertexData;
}

Object* cubeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    Object* cubeVao = newObj(GL_VERTEX_ARRAY);
    glBindVertexArray(cubeVao->id);

    Object* cubeVbo = newObj(GL_ARRAY_BUFFER);
    cubeVao->bufferId = cubeVbo->id;
    glBindBuffer(cubeVbo->target, cubeVbo->id);

    float l = length / 2.0f;

    GLfloat vboData[] = {
        // bot
        midX - l, midY - l, midZ + l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX - l, midY - l, midZ - l, 1.0f,

        // top
        midX - l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX - l, midY + l, midZ - l, 1.0f,

        // front
        midX - l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ + l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f,
        midX - l, midY - l, midZ + l, 1.0f,

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
        midX + l, midY + l, midZ + l, 1.0f,
        midX + l, midY + l, midZ - l, 1.0f,
        midX + l, midY - l, midZ - l, 1.0f,
        midX + l, midY - l, midZ + l, 1.0f
    };
    glBufferData(cubeVbo->target, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    free(cubeVbo);

    return cubeVao;
}

Object* planeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    Object* planeVao = newObj(GL_VERTEX_ARRAY);
    glBindVertexArray(planeVao->id);

    Object* planeVbo = newObj(GL_ARRAY_BUFFER);
    planeVao->bufferId = planeVbo->id;
    glBindBuffer(planeVbo->target, planeVbo->id);

    float l = length / 2.0f;

    GLfloat vboData[] = {
        midX - l, midY, midZ + l, 1.0f,
        midX + l, midY, midZ + l, 1.0f,
        midX + l, midY, midZ - l, 1.0f,
        midX - l, midY, midZ - l, 1.0f,

        0.0f, 0.0f,
        texRes, 0.0f,
        texRes, texRes,
        0.0f, texRes
    };
    glBufferData(planeVbo->target, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    glEnableVertexAttribArray(1);

    free(planeVbo);

    return planeVao;
}


