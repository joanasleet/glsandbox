#include "Mesh.h"
#include "Util.h"
#include "string.h"
#include "Engine.h"
#include "MatrixMath.h"

extern Engine* renderer;

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

void planeVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

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
        -l, 0.0f, +l,
        +l, 0.0f, +l,
        +l, 0.0f, -l,
        -l, 0.0f, -l,

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 12));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * (12 + 8)));
    glEnableVertexAttribArray(2);

    /* physics attr */
    float faces[4][3] = {
        { -l, 0.0f, +l },
        { +l, 0.0f, +l },
        { +l, 0.0f, -l },
        { -l, 0.0f, -l }
    };
    NewtonCollision *ncol = ( mesh->ncol = NewtonCreateTreeCollision( renderer->nworld, 0 ) );
    NewtonTreeCollisionBeginBuild( ncol );
    NewtonTreeCollisionAddFace( ncol, 4, &faces[0][0], 3*sizeof(float), 0 );
    NewtonTreeCollisionEndBuild( ncol, 0 );
}

void cubeInVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 24;

    float l = size / 2.0f;

    GLfloat vboData[] = {
        
        // bot
        -l, -l, +l,
        +l, -l, +l,
        +l, -l, -l,
        -l, -l, -l,

        // top
        -l, +l, +l,
        -l, +l, -l,
        +l, +l, -l,
        +l, +l, +l,

        // front
        -l, +l, +l,
        +l, +l, +l,
        +l, -l, +l,
        -l, -l, +l,

        // back
        -l, -l, -l,
        +l, -l, -l,
        +l, +l, -l,
        -l, +l, -l,

        // left
        -l, -l, +l,
        -l, -l, -l,
        -l, +l, -l,
        -l, +l, +l,

        // right
        +l, +l, +l,
        +l, +l, -l,
        +l, -l, -l,
        +l, -l, +l
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (vboData), vboData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    /* physics attr */
    mesh->ncol = NewtonCreateBox( renderer->nworld, size, size, size, 0, NULL );
}

void cubeOutVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 24;

    float l = size / 2.0f;

    GLfloat vboData[] = {
       
        // bot
         -l, -l, -l,
         +l, -l, -l,
         +l, -l, +l,
         -l, -l, +l,

         // top
         +l, +l, +l,
         +l, +l, -l,
         -l, +l, -l,
         -l, +l, +l,

         // front
         -l, -l, +l,
         +l, -l, +l,
         +l, +l, +l,
         -l, +l, +l,

         // back
         -l, +l, -l,
         +l, +l, -l,
         +l, -l, -l,
         -l, -l, -l,

         // left
         -l, +l, +l,
         -l, +l, -l,
         -l, -l, -l,
         -l, -l, +l,

         // right
         +l, -l, +l,
         +l, -l, -l,
         +l, +l, -l,
         +l, +l, +l,

        // texture coords
        0.0f, texres,
        texres, texres,
        texres, 0.0f,
        0.0f, 0.0f,

        0.0f, texres,
        texres, texres,
        texres, 0.0f,
        0.0f, 0.0f,

        0.0f, texres,
        texres, texres,
        texres, 0.0f,
        0.0f, 0.0f,

        0.0f, texres,
        texres, texres,
        texres, 0.0f,
        0.0f, 0.0f,

        0.0f, texres,
        texres, texres,
        texres, 0.0f,
        0.0f, 0.0f,

        0.0f, texres,
        texres, texres,
        texres, 0.0f,
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 72));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * (72 + 48)));
    glEnableVertexAttribArray(2);

    /* physics attr */
    mesh->ncol = NewtonCreateBox( renderer->nworld, size, size, size, 0, NULL );
}

#define WRITE_TRIANGLE( A, B, C, target, indx ) \
    target[indx+0] = A[0]; \
    target[indx+1] = A[1]; \
    target[indx+2] = A[2]; \
    target[indx+3] = B[0]; \
    target[indx+4] = B[1]; \
    target[indx+5] = B[2]; \
    target[indx+6] = C[0]; \
    target[indx+7] = C[1]; \
    target[indx+8] = C[2]; \

GLfloat* isosphereIn( GLfloat, GLfloat, int, size_t*, int* ); 
GLfloat* isosphereIn( GLfloat radius, GLfloat texres, int res, size_t *size, int* vertCount ) {

    /* three conjoined, orthogonal golden rectangles = icosahedron
     *
     * -------
     * |     |
     * |     |  y
     * |     | 
     * -------
     *    x     */

    /* ( x : y = 1 : 1.618 )  =  golden ratio */
    const float x = 1.0f;
    const float y = 1.618034f * x;

    /* base icosahedron data */
    const float baseData[] = {

         -x, +y, 0.0f,
         +x, +y, 0.0f,
         -x, -y, 0.0f,
         +x, -y, 0.0f,

         0.0f, -x, +y,
         0.0f, +x, +y,
         0.0f, -x, -y,
         0.0f, +x, -y,

         +y, 0.0f, -x,
         +y, 0.0f, +x,
         -y, 0.0f, -x,
         -y, 0.0f, +x
    };

    /* base icosahedron 
     * clockwise winding */
    const int indices[] = {
        0, 5,  11,
        0, 1,  5,
        0, 7,  1,
        0, 10, 7,
        0, 11, 10,

        1,  9, 5,
        5,  4, 11,
        11, 2, 10,
        10, 6, 7,
        7,  8, 1,

        3, 4, 9,
        3, 2, 4,
        3, 6, 2,
        3, 8, 6,
        3, 9, 8,

        4, 5,  9,
        2, 11, 4,
        6, 10, 2,
        8, 7,  6,
        9, 1,  8
    };

    /* base triangles */
    const int bsize = 20;

    /* vertices per triangle */
    const int vpt = 3;

    /* base vertex component count */
    const int vcomps = 3;

    /* new triangles per triangle */
    const int rtria = 4;

    /*  vertices = 20 base triangles * 4 per refinement * 3 vertices per
     *  triangle */
    int vertices = bsize * vpt * powf( rtria, res );
    int dsize = vertices*vcomps;
    *vertCount = vertices;
    *size = sizeof( GLfloat ) * dsize;

    GLfloat *finalData = alloc( GLfloat, vertices*vcomps );

    /* write with gaps between triangles
     * inside buffer get smaller with 
     * higher iteration */
    int tgap = powf( rtria, res );

    /* triangle */
    for( int t = 0; t < bsize; t++ ) { 

        /* vertex */
        for( int v = 0; v < vpt; ++v ) {

            int indx = indices[vpt*t+v];

            /* component */
            for( int c = 0; c < vcomps; ++c ) {
                finalData[tgap*vcomps*vpt*t+vcomps*v+c] = baseData[vcomps*indx+c];
            }
        }
    }
 
    /* interation size */
    int isize = bsize;

    /* do refinement */
    for( int r = 0; r < res; ++r ) {

        /* calc new triangle data */
        for( int t = 0; t < isize; ++t ) {

            /*              B
             *             /\
             *            /  \
             *           /    \
             *          /      \
             *       a /________\ b
             *        /\        /\
             *       /  \      /  \
             *      /    \    /    \
             *     /      \  /      \
             *    /        \/        \
             *  A --------- c -------- C
             */
            
            int tindx = tgap*vcomps*vpt*t;
            GLfloat A[] = { finalData[tindx+0], finalData[tindx+1], finalData[tindx+2] };
            GLfloat B[] = { finalData[tindx+3], finalData[tindx+4], finalData[tindx+5] };
            GLfloat C[] = { finalData[tindx+6], finalData[tindx+7], finalData[tindx+8] };

            /* new triangle vertices = midpoint of two original vertices */
            GLfloat a[3];
            vec3add( A, B, a );
            vec3scale( 0.5f, a );

            GLfloat b[3];
            vec3add( B, C, b );
            vec3scale( 0.5f, b );

            GLfloat c[3];
            vec3add( C, A, c );
            vec3scale( 0.5f, c );

            /* spherical displacement: OutputVec = midV + radius*normalize( InputVec - midV ) */
            GLfloat origin[] = { 0.0f, 0.0f, 0.0f };
            sphereMap( A, origin, radius );
            sphereMap( B, origin, radius );
            sphereMap( C, origin, radius );
            sphereMap( a, origin, radius );
            sphereMap( b, origin, radius );
            sphereMap( c, origin, radius );
            
            /* write into buffer */
            int wtgap = tgap / rtria;
            int triaOffset = wtgap*vcomps*vpt;
            WRITE_TRIANGLE( A, a, c, finalData, tindx+triaOffset*0 );
            WRITE_TRIANGLE( a, B, b, finalData, tindx+triaOffset*1 );
            WRITE_TRIANGLE( c, b, C, finalData, tindx+triaOffset*2 );
            WRITE_TRIANGLE( a, b, c, finalData, tindx+triaOffset*3 );
        }
        isize = bsize * powf( rtria, r+1 );
        tgap /= rtria;
    }

    return finalData;
}

void sphereInVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO( vao );
    VBO( vbo, GL_ARRAY_BUFFER );

    mesh->vaoId = vao;
    mesh->vboId = vbo;

    /* gen sphere buffer */
    size_t dsize;
    int vertices;
    GLfloat* data = isosphereIn( size, texres, 7, &dsize, &vertices );

    /* gen texture coords */

    /* gen normals */

    /* merge buffers */

    /* send buffer to gpu */
    glBufferData( GL_ARRAY_BUFFER, dsize, data, GL_STATIC_DRAW );

    /*
     * set data format */

    /* position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    /* texcoords */
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    //glEnableVertexAttribArray(1);

    /* normals  */
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    //glEnableVertexAttribArray(2);

    mesh->count = vertices;
    
    /* physics attr */
    mesh->ncol = NewtonCreateSphere( renderer->nworld, size, 0, NULL );
}

GLfloat* isosphereOut( GLfloat, GLfloat, int, size_t*, int* ); 
GLfloat* isosphereOut( GLfloat radius, GLfloat texres, int res, size_t *size, int* vertCount ) {


    /* three conjoined, orthogonal golden rectangles = icosahedron
     *
     * -------
     * |     |
     * |     |  b
     * |     | 
     * -------
     *    a     */

    /* ( a : b = 1 : 1.618 )  =  golden ratio */
    const float x = 1.0f;
    const float y = 1.618034f * x;

    /* base icosahedron data */
    const float baseData[] = {
         -x, +y, 0.0f,
         +x, +y, 0.0f,
         -x, -y, 0.0f,
         +x, -y, 0.0f,

         0.0f, -x, +y,
         0.0f, +x, +y,
         0.0f, -x, -y,
         0.0f, +x, -y,

         +y, 0.0f, -x,
         +y, 0.0f, +x,
         -y, 0.0f, -x,
         -y, 0.0f, +x
    };

    /* base icosahedron
     * counter clockwise winding */
    const int indices[] = {
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

    /* base triangles */
    const int bsize = 20;

    /* vertices per triangle */
    const int vpt = 3;

    /* base vertex component count */
    const int vcomps = 3;

    /* new triangles per triangle */
    const int rtria = 4;

    /*  vertices = 20 base triangles * 4 per refinement * 3 vertices per
     *  triangle */
    int vertices = bsize * vpt * ( int )powf( rtria, res );
    int dsize = vertices*vcomps;
    *vertCount = vertices;
    *size = sizeof( GLfloat ) * ( size_t ) dsize;

    GLfloat *finalData = alloc( GLfloat, vertices*vcomps );

    /* write with gaps between triangles
     * inside buffer get smaller with 
     * higher iteration */
    int tgap = (int) powf( rtria, res );

    /* triangle */
    for( int t = 0; t < bsize; t++ ) { 

        /* vertex */
        for( int v = 0; v < vpt; ++v ) {

            int indx = indices[vpt*t+v];

            /* component */
            for( int c = 0; c < vcomps; ++c ) {
                finalData[tgap*vcomps*vpt*t+vcomps*v+c] = baseData[vcomps*indx+c];
            }
        }
    }
 
    /* interation size */
    int isize = bsize;

    /* do refinement */
    for( int r = 0; r < res; ++r ) {

        /* calc new triangle data */
        for( int t = 0; t < isize; ++t ) {

            /*              C
             *             /\
             *            /  \
             *           /    \
             *          /      \
             *       c /________\ b
             *        /\        /\
             *       /  \      /  \
             *      /    \    /    \
             *     /      \  /      \
             *    /        \/        \
             *  A --------- a -------- B
             */
            
            int tindx = tgap*vcomps*vpt*t;

            /* original triangle vertices */
            GLfloat A[] = { finalData[tindx+0], finalData[tindx+1], finalData[tindx+2] };
            GLfloat B[] = { finalData[tindx+3], finalData[tindx+4], finalData[tindx+5] };
            GLfloat C[] = { finalData[tindx+6], finalData[tindx+7], finalData[tindx+8] };

            /* new triangle vertices = midpoint of two original vertices */
            GLfloat a[3];
            vec3add( A, B, a );
            vec3scale( 0.5f, a );

            GLfloat b[3];
            vec3add( B, C, b );
            vec3scale( 0.5f, b );

            GLfloat c[3];
            vec3add( C, A, c );
            vec3scale( 0.5f, c );

            /* spherical displacement: OutputVec = midV + radius*normalize( InputVec - midV ) */
            GLfloat origin[] = { 0.0f, 0.0f, 0.0f };
            sphereMap( A, origin, radius );
            sphereMap( B, origin, radius );
            sphereMap( C, origin, radius );
            sphereMap( a, origin, radius );
            sphereMap( b, origin, radius );
            sphereMap( c, origin, radius );
            
            /* write into buffer */
            int wtgap = tgap / rtria;
            int triaOffset = wtgap*vcomps*vpt;
            WRITE_TRIANGLE( A, a, c, finalData, tindx+triaOffset*0 );
            WRITE_TRIANGLE( a, B, b, finalData, tindx+triaOffset*1 );
            WRITE_TRIANGLE( c, b, C, finalData, tindx+triaOffset*2 );
            WRITE_TRIANGLE( a, b, c, finalData, tindx+triaOffset*3 );
        }
        isize = (int) ( bsize * powf( rtria, r+1 ) );
        tgap /= rtria;
    }

    return finalData;
}

void sphereOutVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO( vao );
    VBO( vbo, GL_ARRAY_BUFFER );

    mesh->vaoId = vao;
    mesh->vboId = vbo;

    /* gen sphere buffer */
    size_t dsize;
    int vertices;
    GLfloat* data = isosphereOut( size, texres, 7, &dsize, &vertices );

    /* gen texture coords */

    /* gen normals */

    /* merge buffers */

    /* send data to gpu */
    glBufferData( GL_ARRAY_BUFFER, dsize, data, GL_STATIC_DRAW );

    /*
     * set data format */

    /* position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    /* texcoords */
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    //glEnableVertexAttribArray(0);

    /* normals  */
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    //glEnableVertexAttribArray(0);

    mesh->count = vertices;

    /* physics attr */
    mesh->ncol = NewtonCreateSphere( renderer->nworld, size, 0, NULL );
}

void terrainVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    int vcomps = 3;
    int patch_dim = 10;
    int patch_verts = 4;

    int tria = patch_verts * vcomps;

    float plen = size / (float) patch_dim;
    float hlen = size / 2.0f;

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = patch_dim * patch_dim * patch_verts;

    GLfloat data[(mesh->count) * vcomps];

    for (int i = 0; i < patch_dim; ++i) {

        for (int j = 0; j < patch_dim; ++j) {

            data[ (patch_dim * i + j)*tria + 0] = -hlen + j * (plen);
            data[ (patch_dim * i + j)*tria + 1] = 0.0f;
            data[ (patch_dim * i + j)*tria + 2] = -hlen + i * (plen);
 
            data[ (patch_dim * i + j)*tria + 3] = -hlen + j * (plen) + plen;
            data[ (patch_dim * i + j)*tria + 4] = 0.0f;
            data[ (patch_dim * i + j)*tria + 5] = -hlen + i * (plen);

            data[ (patch_dim * i + j)*tria + 6] = -hlen + j * (plen) + plen;
            data[ (patch_dim * i + j)*tria + 7] = 0.0f;
            data[ (patch_dim * i + j)*tria + 8] = -hlen + i * (plen) + plen;

            data[ (patch_dim * i + j)*tria + 9] = -hlen + j * (plen);
            data[ (patch_dim * i + j)*tria + 10] = 0.0f;
            data[ (patch_dim * i + j)*tria + 11] = -hlen + i * (plen) + plen;
        }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, patch_verts);
}

void overlayVAO( GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;
    mesh->count = 4;

    float l = size / 2.0f;

    const GLfloat data[] = {

        -l, -l, 0.0f,
        +l, -l, 0.0f,
        +l, +l, 0.0f,
        -l, +l, 0.0f,

        0.0f, 0.0f,
        texres, 0.0f,
        texres, texres,
        0.0f, texres
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof (data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof (GLfloat) * 12));
    glEnableVertexAttribArray(1);
}

#define IARATIO 0.5625f
#define ASCII_OFFSET 32
#define FONTMAP_SIZE 16
#define FONTMAP_SIZE_F 16.0f

// TODO: redo this shit
void staticTextVAO( const char* text, GLfloat size, GLfloat texres, Mesh *mesh ) {

    VAO(vao);
    VBO(vbo, GL_ARRAY_BUFFER);

    mesh->vaoId = vao;
    mesh->vboId = vbo;

    uint64 strl = strlen(text);

    // font size scale factor
    size /= 150.0f;

    // vertex per char
    uint64 vertices = 4 * strl;
    mesh->count = ( int32 ) vertices;

    // tex + position coords
    vertices *= (2 + 4);
    GLfloat data[vertices];

    GLfloat xstep = size * IARATIO;

    char c, indx;
    GLfloat texstep;
    GLfloat xleft, xright, ybot /*, ytop */;
    GLfloat textopleftX, textopleftY;

    info("Generating %lu quads for text '%s'", strl, text);
    for ( uint64 i = 0; i < strl; ++i) {

        xleft = i * xstep;
        xright = (i + 1) * xstep;

        /* ytop = y */
        ybot = -size;

        // top left
        data[16 * i] = xleft;
        data[16 * i + 1] = 0.0f;
        data[16 * i + 2] = 0.0f;
        data[16 * i + 3] = 1.0f;

        // top right
        data[16 * i + 4] = xright;
        data[16 * i + 5] = 0.0f;
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

        printf("#%lu quad tex coords\n", i);

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

        printf( "- - -\n" );
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(GLfloat) * 16 * strl));
    glEnableVertexAttribArray(1);
}

