#include "GLHelper.h"

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

Object* cubeMap(const char* cubeFaces[]) {

    Object* cubeTex = newObj(GL_TEXTURE_CUBE_MAP);
    glBindTexture(cubeTex->target, cubeTex->id);

    int w, h, c;

    unsigned char* faceTexBuffer;

    for (int face = 0; face < 6; face++) {
        faceTexBuffer = texBuffer(cubeFaces[face], &w, &h, &c);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, faceTexBuffer);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return cubeTex;
}
