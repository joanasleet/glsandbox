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

/* shader stage strings */
const char* ShaderName[] = {
    "Vertex",
    "Fragment",
    "TesselationCont",
    "TesselationEval",
    "Geometry",
    "Compute"
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
    sphereInVAO,
    sphereOutVAO,
    cubeInVAO,
    cubeOutVAO,
    overlayVAO,
    terrainVAO
};

/* draw call */
DrawFunc DrawFuncs[] = {
    drawArrays,     /* Plane */
    drawArrays,     /* SphereIn */
    drawArrays,     /* SphereOut */
    drawArrays,     /* CubeIn */
    drawArrays,     /* CubeOut */
    drawArrays,     /* Overlay */
    drawArrays      /* Terrain */
};

GLuint DrawMode[] = {
    GL_QUADS,     /* Plane */
    GL_TRIANGLES, /* SphereIn */
    GL_TRIANGLES, /* SphereOut */
    GL_QUADS,     /* CubeIn */
    GL_QUADS,     /* CubeOut */
    GL_QUADS,     /* Overlay */
    GL_PATCHES    /* Terrain */
};


/*
 * physics lookups */

PhysicsBodyCF PhysicsBodyFuncs[] = {
    NewtonCreateDynamicBody,
    NewtonCreateKinematicBody,
    NewtonCreateDeformableBody
};

