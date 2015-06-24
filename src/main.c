#include "Util.h"
#include "Engine.h"
#include "MatrixMath.h"
#include "SceneManager.h"

#include <unistd.h>
#include <string.h>

FILE *errlog;
FILE *infolog;
FILE *watchlog;

Engine *renderer;

int main(int argc, char **argv) {

    clear_logs();

    log_info("%s\n", "- - - - - - - - - - Log Start - - - - - - - - - -");

    renderer = init();
    loadScene(renderer);

    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}
