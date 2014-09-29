#include "Engine.h"
#include "SceneManager.h"
#include "Debugger.h"
#include "Script.h"

#include <string.h>
#include <limits.h>

using namespace std;

FILE* scrolllog;
FILE* watchlog;

Engine* renderer;

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

    //    unsigned char n = 3;
    //
    //    const char** strings = (const char**) malloc(sizeof (const char*)*n);
    //
    //    strings[0] = "Test";
    //    strings[1] = "Hello";
    //    strings[2] = "World";
    //
    //    for (int i = 0; i < n; i++) {
    //        info("%s", strings[i]);
    //    }
    //
    //    const char* src = "copy successful!";
    //    size_t s = strlen(src);
    //    size_t s2 = sizeof (src);
    //    info("SRC: %lu chars of size %lu", s, s2);
    //    char* dest = (char*) malloc(sizeof (char)*(s + 1));
    //    strcpy(dest, src);
    //    info("%s -> %s", src, dest);
    //    info("DEST: %lu chars of size %lu", strlen(dest), sizeof (dest));
    //    info("%lu", sizeof (int));
    //
    //    for (int i = 0; i < 16; i++) {
    //        info("(i=%d) %lu [%c]", i, sizeof (src[i]), src[i]);
    //    }
    //    
    //    info("%d", CHAR_BIT);
    //    info("%lu", sizeof("abc"));
    //
    //
    //    free(strings);
    //
    //    return 0;

    clear_syserr();
    clear_logs();

    renderer = init();
    loadScene(renderer);

    return 0;

    enterLoop(renderer);
    check_syserr();

    terminate(renderer);
    check_syserr();

    return EXIT_SUCCESS;
}

/* SAMPLE MESH:
 
    Mesh* overlay = newMesh();
    const char* uniforms1[] = {"MVP"};
    void (*funcs1[])(GLint, Camera*) = {MVPnoTrans};
    const char* shaders2[] = {"overlay.vert", "overlay.frag"};

    overlay->vaoId = overlayVAO();
    overlay->tex = newTexture("fontsheet/font.png");
    overlay->uniforms = uniforms1;
    overlay->setUniformFunc = funcs1;
    overlay->uniLen = 0; // !!!
    overlay->shaders = shaders2;
    overlay->shadersLen = 2;
    overlay->drawFunc = drawArrays;
    overlay->mode = GL_QUADS;
    overlay->first = 0;
    overlay->count = 4;
 
 */