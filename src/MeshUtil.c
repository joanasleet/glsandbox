#include "MeshUtil.h"
#include "Debugger.h"
#include "Deallocator.h"

GLuint DrawMode[] = {
    GL_QUADS, GL_QUADS, GL_TRIANGLE_FAN, GL_QUADS, GL_QUADS, GL_PATCHES
};

GLsizei VertexCount[] = {
    4, 24, 8004, 24, 4, 4
};

GLuint genVao(VaoType type, GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    switch (type) {
        case PLANE:
        return planeVAO(length, texRes, midX, midY, midZ);
        case CUBE:
        return cubeVAO(length, texRes, midX, midY, midZ);
        case SPHERE:
        return sphereVAO(length, texRes, midX, midY, midZ);
        case CUBEMAP:
        return cubeMapVAO(length, texRes, midX, midY, midZ);
        case OVERLAY:
        return overlayVAO();
        case TERRAIN:
        return terrainVAO(length, midX, midY, midZ);
        default:
        err("Unknown vao type: %d", type);
        return 0;
    }
}

GLuint planeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

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

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*(16 + 8)));
    glEnableVertexAttribArray(2);

    return vao;
}

GLuint cubeMapVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    float l = length / 2.0f;

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

GLuint sphereVAO(GLfloat radius, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    radius = 10;
    
    int n = 1000;
    int size = 4 + 4*(2*n);
    int hsize = 2*(2*n);
    float data[size];
    
    float y = 0.0f;
    float x = -radius;
    float xstep = (2.0f*radius)/(n-1);

    // triangle fan ref point
    data[0] = 0.0f;
    data[1] = 0.0f;
    data[2] = 0.0f;
    data[3] = 1.0f;

    for (int i = 4; i <= hsize; i += 4) {
        
        y = sqrtf( abs(radius*radius - x*x) );
        //fprintf(stderr, "[x = %f]\t[y = %f]\n", x, y);

        // upper sphere
        data[i] = x;
        data[i+1] = y;
        data[i+2] = 0.0f;
        data[i+3] = 1.0f;

        // lower sphere
        data[size-i] = x;
        data[size-i+1] = -y;
        data[size-i+2] = 0.0f;
        data[size-i+3] = 1.0f;

        x += xstep;
    }

      
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    //glEnableVertexAttribArray(1);

    return vao;
}

GLuint cubeVAO(GLfloat length, GLfloat texRes, GLfloat midX, GLfloat midY, GLfloat midZ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    float l = length / 2.0f;

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

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*96));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*(96 + 48)));
    glEnableVertexAttribArray(2);

    return vao;
}



GLuint terrainVAO(GLfloat length, GLfloat midX, GLfloat midY, GLfloat midZ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    GLfloat len = length / 2.0f;

    const GLfloat data[] = {

        midX - len, midY, midZ + len, 1.0f,
        midX - len, midY, midZ - len, 1.0f,
        midX + len, midY, midZ - len, 1.0f,
        midX + len, midY, midZ + len, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    glEnableVertexAttribArray(1);

    return vao;
}

GLuint overlayVAO() {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

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

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    glEnableVertexAttribArray(1);

    return vao;
}
