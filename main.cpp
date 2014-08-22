#include "Engine.h"
#include "MeshUtil.h"

using namespace std;

Engine* renderer = init();

int main(int argc, char** argv) {

    /* TERRAIN */
    Mesh* terrain = newMesh();
    const char* uniforms0[] = {"P", "MV"};
    void (*funcs0[])(GLint, Camera*) = {P, MV};
    const char* shaders0[] = {"base_tess.vert", "tex.frag", "control.tess", "eval.tess"};

    terrain->vaoId = terrainVAO(10000.0f, 10000.0f);
    terrain->tex = newTexture("textures/white.png");
    terrain->uniforms = uniforms0;
    terrain->setUniformFunc = funcs0;
    terrain->uniLen = 2;
    terrain->shaders = shaders0;
    terrain->shadersLen = 4;
    terrain->drawFunc = drawArrays;
    terrain->mode = GL_PATCHES;
    terrain->first = 0;
    terrain->count = 4;
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    add(terrain, renderer);

    /* SKYBOX */
    Mesh* skybox = newMesh();
    const char* uniforms1[] = {"MVP"};
    void (*funcs1[])(GLint, Camera*) = {MVPnoTrans};
    const char* shaders1[] = {"cubemap.vert", "cubemap.frag"};

    const char* cubefaces[] = {
        "textures/xn.bmp",
        "textures/xp.bmp",
        "textures/yp.bmp",
        "textures/yn.bmp",
        "textures/zp.bmp",
        "textures/zn.bmp"
    };

    skybox->vaoId = cubeMapVAO(20000.0f);
    skybox->tex = cubeMap(cubefaces);
    skybox->uniforms = uniforms1;
    skybox->setUniformFunc = funcs1;
    skybox->uniLen = 1;
    skybox->shaders = shaders1;
    skybox->shadersLen = 2;
    skybox->drawFunc = drawArrays;
    skybox->mode = GL_QUADS;
    skybox->first = 0;
    skybox->count = 24;
    add(skybox, renderer);


    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}