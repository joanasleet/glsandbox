#include "GLHelper.h"

#include <math.h>

unsigned char* texture(const char* file, int* width, int* height, int* compression, int channels) {

    unsigned char* image_data = stbi_load(file, width, height, compression, channels);

    if (!image_data) {
        ERR("Failed to load texture from file <%s>", file);
    }
    INFO("Texture attribute: %i x %i (%i) <%s>", *width, *height, *compression, file);

    return image_data;
}

GLfloat* cubeVD(GLsizei* size, float length, float midX, float midY, float midZ, bool genTexels) {

    GLfloat* vertexData;
    GLsizei bufferSize;

    if (genTexels) {
        bufferSize = sizeof (GLfloat)*((8 * 4) + (8 * 2));
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

    return vertexData;
}

GLfloat* planeVD(GLsizei* size, GLfloat length, GLfloat midX, GLfloat midY, GLfloat midZ, bool genTexels) {

    GLfloat* vertexData;
    GLsizei bufferSize;

    if (genTexels) {
        bufferSize = sizeof (GLfloat)*((4 * 4) + (4 * 2));
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

        vertexData[18] = 10.0f;
        vertexData[19] = 10.0f;

        vertexData[20] = 0.0f;
        vertexData[21] = 10.0f;

        vertexData[22] = 0.0f;
        vertexData[23] = 0.0f;
    }

    return vertexData;
}