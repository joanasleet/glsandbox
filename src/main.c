#include "Engine.h"
#include "SceneManager.h"
#include "Debugger.h"
#include "Script.h"
#include "MatrixMath.h"

FILE* scrolllog;
FILE* watchlog;

Engine* renderer;

/* progress TODO:
- overlay with fonts
- fix timestep
- add shader uniforms defaults
- better architecture
- atmospheric scattering
- simple terrain
 */

int main(int argc, char** argv) {

    clear_syserr();
    clear_logs();

    renderer = init();
    loadScene(renderer);

    enterLoop(renderer);
    
    terminate(renderer);

    return EXIT_SUCCESS;
}