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

float force[] = { 0.0f, 0.0f, 0.0f };
float torque[] = { 0.0f, 0.0f, 0.0f };

int main(int argc, char **argv) {

    clear_logs();

    log_info("%s\n", "- - - - - - - - - - log Start - - - - - - - - - -");

    renderer = init();
    loadScene(renderer);

    genTextures( renderer );
    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}
