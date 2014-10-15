#include "common.h"

#include "MeshUtil.h"
#include "Debugger.h"
#include "SceneManager.h"
#include "Script.h"
#include "Texture.h"

#define SCENE_LOADER "scripts/loadScene.lua"

void loadScene(Engine* renderer) {
    return_guard(renderer, RVOID);

    initScript(SCENE_LOADER);
    exeScript();

    uint32 meshCount = (uint32) popInt();

    info("Found meshes: %d", meshCount);
    return_guard(meshCount, RVOID);

    Mesh** meshes = (Mesh**) malloc(sizeof (Mesh*) * meshCount);

    Mesh* mesh;
    for (uint32 j = 0; j < meshCount; j++) {
        mesh = newMesh();

        /* mesh name */
        info("Adding mesh: %s", popString());


        /* vao type */
        uint32 vaoType = (uint32) popInt();
        mesh->vaoId = genVao((VaoType) vaoType, 100000.0f, 1000.0f, 0.0f, 0.0f, 0.0f);

        // TODO: read vao params from scene

        /* texture */

        /* TODO
         * check for multiple texture maps */

        const char* diffMap = popString();
        Material* mat = newMaterial();
        mat->diffuseMap = newTex(diffMap);
        mesh->mats = mat;

        /* uniform count */
        uint8 uniformCount = (uint8) popInt();
        mesh->uniLen = uniformCount;

        /* uniforms */
        mesh->uniforms = (const char**) malloc(sizeof (const char*)*uniformCount);

        const char* src;
        char* dest;
        for (uint8 i = 0; i < uniformCount; i++) {

            /* i-th uniform variable */
            src = popString();

            dest = (char*) malloc(sizeof (char)*(strlen(src) + 1));
            mesh->uniforms[i] = (const char*) strcpy(dest, src);
            info("Uniform variable: %s", mesh->uniforms[i]);
        }

        /* uniforms setter functions */
        UniformVarFunc *uniVarFuncs = (UniformVarFunc*) malloc(sizeof (UniformVarFunc) * uniformCount);
        for (uint8 i = 0; i < uniformCount; i++) {
            uniVarFuncs[i] = uniVarFuncLUT[popInt()];
        }
        mesh->setUniformFunc = uniVarFuncs;

        /* shader count */
        uint8 shaderCount = (uint8) popInt();
        mesh->shadersLen = shaderCount;

        /* shaders */
        mesh->shaders = (const char**) malloc(sizeof (const char*)*shaderCount);

        for (uint8 i = 0; i < shaderCount; i++) {

            /* i-th shader */
            src = popString();

            dest = (char*) malloc(sizeof (char)*(strlen(src) + 1));
            mesh->shaders[i] = (const char*) strcpy(dest, src);
            info("Shader: %s", mesh->shaders[i]);
        }

        mesh->drawFunc = drawArrays;
        mesh->first = 0;
        mesh->count = VertexCount[vaoType];
        mesh->mode = DrawMode[vaoType];

        meshes[j] = mesh;
    }

    termScript();

    renderer->meshCount = meshCount;
    renderer->meshes = meshes;
}

void reloadScene(Engine* renderer) {
    return_guard(renderer, RVOID);

    info("%s", "# # # # # Reloading scene # # # # #");
    freeMeshes(renderer);
    clearCache(renderer->shaderCache);
    clearCache(renderer->uniformCache);
    deallocStores();

    loadScene(renderer);
    preloadMeshes(renderer);
    info("%s", "# # # # # Scene reloaded # # # # #");
}
