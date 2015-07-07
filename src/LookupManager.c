#include "LookupManager.h"

/*
 * debug */
const char *ERR_SEVERITY[] = {
    "HIGH",
    "MEDIUM",
    "LOW",
    "NOTE"
};

const char *ERR_SOURCE[] = {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
};

const char *ERR_TYPE[] = {
    "ERROR",
    "DEPRECATED BEHAVIOUR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
};

/* shader stages */
GLenum ShaderType[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER
};

/* uniforms */
UniformSetter UniVarFuncs[] = {
    P,
    MV,
    MVnoTrans,
    MVP,
    MVPnoTrans,
    objMV,
    objMVnoTrans,
    camPos,
    gTime
};

/* vao generator */
VaoFunc VaoFuncs[] = {
    planeVAO,
    sphereVAO,
    cubeInVAO,
    cubeOutVAO,
    overlayVAO,
    terrainVAO
};

/* draw call */
DrawFunc DrawFuncs[] = {
    drawArrays,     /* PLANE */
    drawArrays,   /* SPHERE */
    drawArrays,     /* CUBEIN */
    drawArrays,     /* CUBEOUT */
    drawArrays,     /* OVERLAY */
    drawArrays      /* TERRAIN */
};

GLuint DrawMode[] = {
    GL_QUADS,       /* PLANE */
    GL_TRIANGLES,   /* SPHERE */
    GL_QUADS,       /* CUBEIN */
    GL_QUADS,       /* CUBEOUT */
    GL_QUADS,       /* OVERLAY */
    GL_PATCHES      /* TERRAIN */
};
