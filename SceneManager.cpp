#include "common.h"

#include "SceneManager.h"
#include "Script.h"
#include "Engine.h"

extern Engine* renderer;

void loadScene() {
    if (!renderer) return;
    
    initScript("scripts/loadScene.lua");
    exeScript();
    dumpResult();
    
    termScript();
}

void reloadScene() {

}
