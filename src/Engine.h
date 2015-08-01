#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include "Object.h"
#include "Camera.h"
#include "Context.h"
#include "LuaScript.h"
#include "Newton/include/Newton.h"

#define CONFIG_SCRIPT "scripts/config.lua"

typedef struct {

    uint32 objCount;
    uint32 texCount;

    Camera *mainCam;
    Context *context;

    Object **objects;
    Object **textures;

    Cache *shaderCache;

    Cache *textureCache;
    Cache *texDataCache;

    NewtonWorld *nworld;

} Engine;

/*
 *  primary API */
Engine *init();
void genTextures( Engine* );
void enterLoop( Engine* );
void terminate( Engine* );
void freeTextures( Engine* );
void freeObjects( Engine* );

/* engine intern */
void config( Engine *renderer );
void render(Object *obj, Engine *renderer, float alpha );

#endif
