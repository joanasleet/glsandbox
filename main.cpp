#include "Engine.h"
#include "MeshUtil.h"

using namespace std;

Engine* renderer = init();

int main(int argc, char** argv) {

    /*
    Mesh* terrain = newMesh();
    {
        const char* uniforms[] = {"P", "MV"};
        void (*funcs[])(GLint) = {P, MV};
        const char* shaders[] = {"base_tess.vert", "tex.frag", "control.tess", "eval.tess"};

        terrain->vaoId = terrainVAO(10000.0f, 10000.0f);
        terrain->tex = newTexture("textures/white.png");
        terrain->uniforms = uniforms;
        terrain->setUniformFunc = funcs;
        terrain->uniLen = 2;
        terrain->shaders = shaders;
        terrain->shadersLen = 4;
        terrain->drawFunc = drawArrays;
        terrain->mode = GL_PATCHES;
        terrain->first = 0;
        terrain->count = 4;
        glPatchParameteri(GL_PATCH_VERTICES, 4);
    }
    //preload(terrain);
     * */

    Mesh* plane = newMesh();
    {
        const char* uniforms[] = {"MVP"};
        void (*funcs[])(GLint, Camera*) = {MVP};
        const char* shaders[] = {"base.vert", "tex.frag"};

        plane->vaoId = planeVAO(10000.0f, 100.0f, 0.0f, -50.0f);
        plane->tex = newTexture("textures/sand.png");
        plane->uniforms = uniforms;
        plane->setUniformFunc = funcs;
        plane->uniLen = 1;
        plane->shaders = shaders;
        plane->shadersLen = 2;
        plane->drawFunc = drawArrays;
        plane->mode = GL_QUADS;
        plane->first = 0;
        plane->count = 4;
    };
    add(plane, renderer);

    Mesh* skybox = newMesh();
    {
        const char* uniforms[] = {"MVP"};
        void (*funcs[])(GLint, Camera*) = {MVPnoTrans};
        const char* shaders[] = {"cubemap.vert", "cubemap.frag"};

        const char* cubefaces[] = {
            "textures/xn.bmp",
            "textures/xp.bmp",
            "textures/yp.bmp",
            "textures/yn.bmp",
            "textures/zp.bmp",
            "textures/zn.bmp"
        };

        skybox->vaoId = cubeMapVAO(20000.0f);
        skybox->tex = cubeMap(cubefaces, false, false);
        skybox->uniforms = uniforms;
        skybox->setUniformFunc = funcs;
        skybox->uniLen = 1;
        skybox->shaders = shaders;
        skybox->shadersLen = 2;
        skybox->drawFunc = drawArrays;
        skybox->mode = GL_QUADS;
        skybox->first = 0;
        skybox->count = 24;
    };
    add(skybox, renderer);


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    enterLoop(renderer);

    terminate(renderer);

    return EXIT_SUCCESS;
}