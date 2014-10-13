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

    //float M[16];
    //translate(M, 1, -2, 33.2865f);
    //printMat(M);
       
    //return 0;
    
    clear_syserr();
    clear_logs();

    renderer = init();
    loadScene(renderer);

    enterLoop(renderer);
    check_syserr();

    terminate(renderer);
    check_syserr();

    return EXIT_SUCCESS;
}