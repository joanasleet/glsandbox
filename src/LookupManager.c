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

/* draw call */
GLuint DrawMode[] = {
    GL_QUADS,   /* PLANE */
    GL_QUADS,   /* CUBE */
    GL_PATCHES, /* SPHERE */
    GL_QUADS,   /* CUBEMAP */
    GL_QUADS,   /* OVERLAY */
    GL_PATCHES, /* TERRAIN */
    GL_PATCHES, /* SKYDOME */
    GL_QUADS    /* SCREEN_QUAD */
};
