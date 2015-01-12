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
    P, MV, MVP, MVPnoTrans, objMV, objMVnoTrans
};

/* draw call */
GLuint DrawMode[] = {
    GL_QUADS, GL_QUADS, GL_PATCHES, GL_QUADS, GL_QUADS, GL_PATCHES, GL_PATCHES
};

// VaoType defined in header