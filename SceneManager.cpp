#include "common.h"

#include "SceneManager.h"
#include "Script.h"

void loadScene(Engine* renderer) {
    if (!renderer) return;

    initScript("scripts/loadScene.lua");
    exeScript();
    dumpResult();

    termScript();
}

void reloadScene(Engine* renderer) {

}
