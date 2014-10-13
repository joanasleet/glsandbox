#ifndef ENGINE_H
#define	ENGINE_H

#include "common.h"
#include "Mesh.h"
#include "Camera.h"
#include "Context.h"
#include "ShaderCache.h"

typedef struct Engine {
    Mesh** meshes;
    uint32 meshCount;

    Camera* mainCam;
    Context* glContext;

    Cache* shaderCache;
    Cache* uniformCache;
} Engine;

/*
 *  primary API */
Engine* init();
void enterLoop(Engine* renderer);
void terminate(Engine* renderer);
void freeMeshes(Engine* renderer);

/* engine intern */
void exitIfNoMeshes(Engine* renderer);
void preload(Mesh* mesh, Engine* renderer);
void preloadMeshes(Engine* renderer);
void render(Mesh* mesh, Engine* renderer);
void renderMeshes(Engine* renderer);


#endif	/* ENGINE_H */

