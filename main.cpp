#include "Engine.h"
#include "MeshUtil.h"
#include <lua.h>

using namespace std;

Engine* renderer = init();

/* progress TODO:
- overlay with fonts
- fix timestep
- add shader uniforms defaults
- load scene from text file
- better architecture
- atmospheric scattering
- simple terrain
 */

int main(int argc, char** argv) {

    
    
    /* OVERLAY */
    Mesh* overlay = newMesh();
    //const char* uniforms1[] = {"MVP"};
    //void (*funcs1[])(GLint, Camera*) = {MVPnoTrans};
    const char* shaders2[] = {"overlay.vert", "overlay.frag"};

    overlay->vaoId = overlayVAO();
    overlay->tex = newTexture("fontsheet/font.png");
    //overlay->uniforms = uniforms1;
    //overlay->setUniformFunc = funcs1;
    overlay->uniLen = 0; // !!!
    overlay->shaders = shaders2;
    overlay->shadersLen = 2;
    overlay->drawFunc = drawArrays;
    overlay->mode = GL_QUADS;
    overlay->first = 0;
    overlay->count = 4;
    add(overlay, renderer);

    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}