#ifndef ENGINE_H
#define	ENGINE_H

#include "common.h"
#include "Mesh.h"
#include "Camera.h"
#include "Context.h"
#include "ShaderCache.h"

#define CULL_FACES 0

#define PRINT_WATCH_LOG 1
#define PRINT_CACHE_LOG 1

#define MAX_MESH_COUNT 10

typedef struct Engine {
    Mesh* meshes[MAX_MESH_COUNT];
    unsigned int nextMeshSlot;

    Camera* mainCam;
    Context* glContext;

    ShaderCache* shaderCache;
    UniformCache* uniformCache;
} Engine;

/* main work flow */
Engine* init();
void add(Mesh* mesh, Engine* renderer);
void enterLoop(Engine* renderer);
void terminate(Engine* renderer);

/* engine intern */
void exitIfNoMeshes(Engine* renderer);

void preload(Mesh* mesh, Engine* renderer);
void render(Mesh* mesh, Engine* renderer);

void preloadMeshes(Engine* renderer);
void renderMeshes(Engine* renderer);






#endif	/* ENGINE_H */

