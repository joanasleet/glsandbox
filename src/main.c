#include "main.h"

float force[] = { 0.0f, 0.0f, 0.0f };
float torque[] = { 0.0f, 0.0f, 0.0f };

int main(int argc, char **argv) {

    clear_logs();

    log_info("%s\n", "- - - - - - - - - - Log Start - - - - - - - - - -");

    renderer = init();
    loadScene(renderer);

    genTextures( renderer );
    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}
