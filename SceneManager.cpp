#include "common.h"

#include "MeshUtil.h"
#include "Debugger.h"
#include "SceneManager.h"
#include "Script.h"

void loadScene(Engine* renderer) {
    if (!renderer) return;

    initScript("scripts/loadScene.lua");
    exeScript();
    
    unsigned int meshCount = (unsigned int) popInt();

    Mesh** meshes = (Mesh**) malloc(sizeof (Mesh*) * meshCount);

    Mesh* mesh;
    for (unsigned int j = 0; j < meshCount; j++) {
        mesh = newMesh();

        /* mesh name */
        info("Adding mesh: %s", popString());

        /* vao type */
        mesh->vaoId = genVao((unsigned int) popInt());

        /* texture */
        mesh->tex = newTexture(popString());

        /* uniform count */
        unsigned char uniformCount = (unsigned char) popInt();
        mesh->uniLen = uniformCount;
        /* uniforms */
        mesh->uniforms = (const char**) malloc(sizeof (const char*)*uniformCount);

        const char* src;
        char* dest;
        for (unsigned char i = 0; i < uniformCount; i++) {

            /* i-th uniform variable */
            src = popString();

            dest = (char*) malloc(sizeof (char)*(strlen(src) + 1));
            mesh->uniforms[i] = (const char*) strcpy(dest, src);
            info("Copied string from Lua: %s", mesh->uniforms[i]);
        }


        /* shader count */
        unsigned char shaderCount = (unsigned char) popInt();
        mesh->shadersLen = shaderCount;
        /* shaders */
        mesh->shaders = (const char**) malloc(sizeof (const char*)*shaderCount);

        for (unsigned char i = 0; i < shaderCount; i++) {

            /* i-th shader */
            src = popString();

            dest = (char*) malloc(sizeof (char)*(strlen(src) + 1));
            mesh->shaders[i] = (const char*) strcpy(dest, src);
            info("Copied string from Lua: %s", mesh->shaders[i]);
        }

        mesh->drawFunc = drawArrays;
        mesh->first = 0;
        mesh->count = 24; // should be a lookup
        mesh->mode = GL_QUADS; // should be a lookup

        meshes[j] = mesh;
    }

    termScript();

    renderer->meshCount = meshCount;
    renderer->meshes = meshes;
}

void reloadScene(Engine* renderer) {
    if (!renderer) return;
}
