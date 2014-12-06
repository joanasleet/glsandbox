#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include "Object.h"
#include "Camera.h"
#include "Context.h"
#include "ShaderCache.h"

typedef struct Engine {

    Object **objects;
    uint32 objectCount;

    Camera *mainCam;
    Context *context;

    Cache *shaderCache;
    Cache *uniformCache;
} Engine;

/*
 *  primary API */
Engine *init();
void enterLoop(Engine *renderer);
void terminate(Engine *renderer);
void freeObjects(Engine *renderer);

/* engine intern */
void exitIfNoObjects(Engine *renderer);
void preload(Object *obj, Engine *renderer);
void preloadObjects(Engine *renderer);
void render(Object *obj, Engine *renderer);
void renderObjects(Engine *renderer);

#endif  /* ENGINE_H */