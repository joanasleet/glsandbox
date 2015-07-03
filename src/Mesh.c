#include "Mesh.h"
#include "Util.h"
#include "string.h"

#define IARATIO 0.5625f
#define ASCII_OFFSET 32
#define FONTMAP_SIZE 16
#define FONTMAP_SIZE_F 16.0f

Mesh *newMesh() {

    Mesh *mesh = alloc( Mesh, 1 );
    return_guard(mesh, NULL);
    mesh->first = 0;
    return mesh;
}

void freeMesh(Mesh *mesh) {

    glDeleteBuffers( 1, &( mesh->vboId ) );
    glDeleteVertexArrays( 1, &( mesh->vaoId ) );
    free( mesh );
}

/*
 * draw functions */
void drawArrays(GLenum mode, GLint *first, GLsizei count) {
    glDrawArrays(mode, *first, count);
}

void drawElements(GLenum mode, GLint *first, GLsizei count) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, BUFFER_OFFSET( sizeof(GLint) * first[0] ) );
}

/*
 * vao generators */
void planeVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    /*
     * gen gl objects */
    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    /*
     * store gl objects */
    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 4;

    float l = size / 2.0f;

    GLfloat vboData[] = {

        // vertices
        midx - l, midy, midz + l, 1.0f,
        midx + l, midy, midz + l, 1.0f,
        midx + l, midy, midz - l, 1.0f,
        midx - l, midy, midz - l, 1.0f,

        // texture coords
        0.0f, 0.0f,
        texres, 0.0f,
        texres, texres,
        0.0f, texres,

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
}

void cubeInVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 24;

    float l = size / 2.0f;

    GLfloat vboData[] = {
        // vertices
        // bot
        midx - l, midy - l, midz + l, 1.0f,
        midx + l, midy - l, midz + l, 1.0f,
        midx + l, midy - l, midz - l, 1.0f,
        midx - l, midy - l, midz - l, 1.0f,

        // top
        midx - l, midy + l, midz + l, 1.0f,
        midx - l, midy + l, midz - l, 1.0f,
        midx + l, midy + l, midz - l, 1.0f,
        midx + l, midy + l, midz + l, 1.0f,

        // front
        midx - l, midy + l, midz + l, 1.0f,
        midx + l, midy + l, midz + l, 1.0f,
        midx + l, midy - l, midz + l, 1.0f,
        midx - l, midy - l, midz + l, 1.0f,

        // back
        midx - l, midy - l, midz - l, 1.0f,
        midx + l, midy - l, midz - l, 1.0f,
        midx + l, midy + l, midz - l, 1.0f,
        midx - l, midy + l, midz - l, 1.0f,

        // left
        midx - l, midy - l, midz + l, 1.0f,
        midx - l, midy - l, midz - l, 1.0f,
        midx - l, midy + l, midz - l, 1.0f,
        midx - l, midy + l, midz + l, 1.0f,

        // right
        midx + l, midy + l, midz + l, 1.0f,
        midx + l, midy + l, midz - l, 1.0f,
        midx + l, midy - l, midz - l, 1.0f,
        midx + l, midy - l, midz + l, 1.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
}

void cubeOutVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 24;

    float l = size / 2.0f;

    GLfloat vboData[] = {
        // vertices
        // bot
        midx - l, midy - l, midz - l, 1.0f,
        midx + l, midy - l, midz - l, 1.0f,
        midx + l, midy - l, midz + l, 1.0f,
        midx - l, midy - l, midz + l, 1.0f,

        // top
        midx + l, midy + l, midz + l, 1.0f,
        midx + l, midy + l, midz - l, 1.0f,
        midx - l, midy + l, midz - l, 1.0f,
        midx - l, midy + l, midz + l, 1.0f,

        // front
        midx - l, midy - l, midz + l, 1.0f,
        midx + l, midy - l, midz + l, 1.0f,
        midx + l, midy + l, midz + l, 1.0f,
        midx - l, midy + l, midz + l, 1.0f,

        // back
        midx - l, midy + l, midz - l, 1.0f,
        midx + l, midy + l, midz - l, 1.0f,
        midx + l, midy - l, midz - l, 1.0f,
        midx - l, midy - l, midz - l, 1.0f,

        // left
        midx - l, midy + l, midz + l, 1.0f,
        midx - l, midy + l, midz - l, 1.0f,
        midx - l, midy - l, midz - l, 1.0f,
        midx - l, midy - l, midz + l, 1.0f,

        // right
        midx + l, midy - l, midz + l, 1.0f,
        midx + l, midy - l, midz - l, 1.0f,
        midx + l, midy + l, midz - l, 1.0f,
        midx + l, midy + l, midz + l, 1.0f,

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
}

void sphereVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    VAO( vao );
    VBO( vbo, GL_ARRAY_BUFFER );
    VBO( eab, GL_ELEMENT_ARRAY_BUFFER );

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->eabId = eab;

    // three conjoined, orthogonal golden rectangles = icosahedron
    //
    // -------
    // |     |
    // |     |  b
    // |     | 
    // -------
    //    a    
    //
    //    a : b = 1 : 1.618    =  golden ratio 

    const float a = size/2.0f;
    const float b = 1.618034f * a;

    const float data[] = {
         midx-a, midy+b, midz, 1.0f,
         midx+a, midy+b, midz, 1.0f,
         midx-a, midy-b, midz, 1.0f,
         midx+a, midy-b, midz, 1.0f,

         midx, midy-a, midz+b, 1.0f,
         midx, midy+a, midz+b, 1.0f,
         midx, midy-a, midz-b, 1.0f,
         midx, midy+a, midz-b, 1.0f,

         midx+b, midy, midz-a, 1.0f,
         midx+b, midy, midz+a, 1.0f,
         midx-b, midy, midz-a, 1.0f,
         midx-b, midy, midz+a, 1.0f
    };
    glBufferData( GL_ARRAY_BUFFER, sizeof( data ), data, GL_STATIC_DRAW );

    // base isosahedron
    const unsigned int indices[] = {
        0, 11,  5,
        0,  5,  1,
        0,  1,  7,
        0,  7, 10,
        0, 10, 11,

        1,   5, 9,
        5,  11, 4,
        11, 10, 2,
        10,  7, 6,
        7,   1, 8,

        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        4, 9,  5,
        2, 4, 11,
        6, 2, 10,
        8, 6,  7,
        9, 8,  1
    };

    // increase resolution
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    mesh->count = 60;
}

// TODO: implement midpoint positioning - needs different equation to solve 
void circleVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;

    float radius = size;

    int n = 1000;

    int vertices = n * 2 + 1;
    mesh->count = vertices;

    int dsize = 4 * vertices;
    int hsize = 2 * (2 * n);
    float data[dsize];

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
        data[dsize - i] = x;
        data[dsize - i + 1] = -y;
        data[dsize - i + 2] = 0.0f;
        data[dsize - i + 3] = 1.0f;

        x += xstep;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat)*16));
    //glEnableVertexAttribArray(1);
}


void terrainVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    const int patch_dim = 10;
    const int patch_verts = 4;

    const float plen = size / (float) patch_dim;
    const float hlen = size / 2.0f;

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = patch_dim * patch_dim * patch_verts;

    GLfloat data[(mesh->count) * 4];

    for (int i = 0; i < patch_dim; ++i) {

        for (int j = 0; j < patch_dim; ++j) {

            data[16 * (patch_dim * i + j) + 0] = midx - hlen + j * (plen);
            data[16 * (patch_dim * i + j) + 1] = midy;
            data[16 * (patch_dim * i + j) + 2] = midz - hlen + i * (plen);
            data[16 * (patch_dim * i + j) + 3] = 1.0f;

            data[16 * (patch_dim * i + j) + 4] = midx - hlen + j * (plen) + plen;
            data[16 * (patch_dim * i + j) + 5] = midy;
            data[16 * (patch_dim * i + j) + 6] = midz - hlen + i * (plen);
            data[16 * (patch_dim * i + j) + 7] = 1.0f;

            data[16 * (patch_dim * i + j) + 8] = midx - hlen + j * (plen) + plen;
            data[16 * (patch_dim * i + j) + 9] = midy;
            data[16 * (patch_dim * i + j) + 10] = midz - hlen + i * (plen) + plen;
            data[16 * (patch_dim * i + j) + 11] = 1.0f;

            data[16 * (patch_dim * i + j) + 12] = midx - hlen + j * (plen);
            data[16 * (patch_dim * i + j) + 13] = midy;
            data[16 * (patch_dim * i + j) + 14] = midz - hlen + i * (plen) + plen;
            data[16 * (patch_dim * i + j) + 15] = 1.0f;
        }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, patch_verts);
}

void overlayVAO( GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 4;

    float l = size / 2.0f;

    const GLfloat data[] = {

        midx-l, midy-l, midz, 1.0f,
        midx-l, midy,   midz, 1.0f,
        midx,   midy,   midz, 1.0f,
        midx,   midy-l, midz, 1.0f,

        0.0f,   0.0f,
        0.0f,   texres,
        texres, texres,
        texres, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 16));
    glEnableVertexAttribArray(1);
}

void staticTextVAO( const char* text, GLfloat size, GLfloat texres, GLfloat midx, GLfloat midy, GLfloat midz, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;

    int strl = strlen(text);

    // font size scale factor
    size /= 150.0f;

    // vertex per char
    uint32 vertices = 4 * strl;
    mesh->count = vertices;

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

        xleft = midx + i * xstep;
        xright = midx + (i + 1) * xstep;

        /* ytop = y */
        ybot = midy - size;

        // top left
        data[16 * i] = xleft;
        data[16 * i + 1] = midy;
        data[16 * i + 2] = 0.0f;
        data[16 * i + 3] = 1.0f;

        // top right
        data[16 * i + 4] = xright;
        data[16 * i + 5] = midy;
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
}

