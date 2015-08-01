#ifndef SCENEMANAGER_H
#define	SCENEMANAGER_H

#include "Engine.h"
#include "LuaScript.h"

void loadScene(Engine* renderer);
void reloadScene(Engine* renderer);
void loadObjects( Engine* renderer, script *S );
void loadTextures( Engine* renderer, script *S );

#endif

