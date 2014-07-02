#include "GLHelper.h"

#include <math.h>

unsigned char* texture(const char* file, int* width, int* height, int* compression, int channels) {

    unsigned char* image_data = stbi_load(file, width, height, compression, channels);

    if (!image_data) {
        char str[50];
        sprintf(str, "Failed to load texture from file <%s>", file);
        printError(str);
    }
    fprintf(stdout, "Texture attribute: %i x %i (%i) <%s>\n", *width, *height, *compression, file);

    return image_data;
}

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