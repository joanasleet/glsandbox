#include "LookupManager.h"

/*
 * debug */
const char* ERR_SEVERITY[] = {
    "HIGH",
    "MEDIUM",
    "LOW",
    "NOTE"
};

const char* ERR_SOURCE[] = {
    "API",
    "WINDOW SYSTEM",
    "SHADER COMPILER",
    "THIRD PARTY",
    "APPLICATION",
    "OTHER"
};

const char* ERR_TYPE[] = {
    "ERROR",
    "DEPRECATED BEHAVIOUR",
    "UNDEFINED DEHAVIOUR",
    "PORTABILITY",
    "PERFORMANCE",
    "OTHER"
};

/* GL */
GLenum ShaderType[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER
};

/* uniforms */
UniformVarFunc UniVarFuncs[] = {
    P, MV, MVP, MVPnoTrans
};


/* draw call */
GLuint DrawMode[] = {
    GL_QUADS, GL_QUADS, GL_TRIANGLE_FAN, GL_QUADS, GL_QUADS, GL_PATCHES
};

GLsizei VertexCount[] = {
    4, 24, 8004, 24, 4, 4
};

// VaoType defined in header
