#include "Engine.h"
#include "SceneManager.h"
#include "Debugger.h"
#include "Script.h"
#include "MatrixMath.h"

#include <unistd.h>
#include <string.h>

FILE *errlog;
FILE *infolog;
FILE *watchlog;

Engine *renderer;

/* progress TODO:
- add shader uniforms defaults
- better architecture
- atmospheric scattering
- simple terrain
 */

int main(int argc, char **argv) {

    clear_syserr();
    clear_logs();

    log_info("%s\n", "- - - - - - - - - - Log Start - - - - - - - - - -");

    renderer = init();
    loadScene(renderer);

    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}