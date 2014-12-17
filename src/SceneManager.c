#include "common.h"

#include "Script.h"
#include "MeshUtil.h"
#include "Debugger.h"
#include "Material.h"
#include "SceneManager.h"
#include "LookupManager.h"

#define SCENE_LOADER "scripts/loadScene.lua"

void loadScene(Engine *renderer) {
    return_guard(renderer, RVOID);

    initScript(SCENE_LOADER);
    exeScript();

    // camera
    Camera *cam = renderer->mainCam;

    cam->fov = (float) popFloat();
    cam->targetFov = cam->fov;
    cam->aspectRatio = (float) popFloat();
    cam->state->position[0] = (float) popFloat();
    cam->state->position[1] = (float) popFloat();
    cam->state->position[2] = (float) popFloat();

    uint32 objectCount = (uint32) popInt();

    info("Found objects: %d", objectCount);
    return_guard(objectCount, RVOID);

    // objects
    Object **objects = (Object **) malloc(sizeof (Object *) * objectCount);

    for (uint32 j = 0; j < objectCount; j++) {

        Object *object = newObject();
        Mesh *mesh = newMesh();

        /* object name */
        info("Adding object: %s", popString());

        /* vao type */
        uint32 vaoType = (uint32) popInt();

        float size = popFloat();
        float texres = popFloat();
        float midX = popFloat();
        float midY = popFloat();
        float midZ = popFloat();
        mesh->vaoId = genVao(vaoType, size, texres, midX, midY, midZ, &(mesh->count));

        /* texture */
        int32 texCount = popInt();

        Material *mat = newMaterial();

        switch (texCount) {
        case 1:
            mat->diffuseMap = newTex(popString());
            break;
        case 2:
            mat->diffuseMap = newTex(popString());
            mat->specularMap = newTex(popString());
            break;
        case 3:
            mat->diffuseMap = newTex(popString());
            mat->specularMap = newTex(popString());
            mat->normalMap = newTex(popString());
            break;
        case 4:
            break;
        case 5:
            break;
        case 6: {
            const char *faces[6];
            faces[0] = popString();
            faces[1] = popString();
            faces[2] = popString();
            faces[3] = popString();
            faces[4] = popString();
            faces[5] = popString();

            mat->diffuseMap = cubeTexture((const char **) faces, 0, 0);
        }
        break;
        default:
            break;
        }

        Shader *shader = newShader();
        /* uniform count */
        uint8 uniformCount = (uint8) popInt();
        shader->uniformCount = uniformCount;

        /* uniforms */
        shader->uniforms = (const char **) malloc(sizeof (const char *)*uniformCount);

        const char *src;
        char *dest;
        for (uint8 i = 0; i < uniformCount; i++) {

            /* i-th uniform variable */
            src = popString();

            dest = (char *) malloc(sizeof (char) * (strlen(src) + 1));
            shader->uniforms[i] = (const char *) strcpy(dest, src);
            info("Uniform variable: %s", shader->uniforms[i]);
        }

        /* uniforms setter functions */
        UniformSetter *setters = (UniformSetter *) malloc(sizeof (UniformSetter) * uniformCount);
        for (uint8 i = 0; i < uniformCount; i++) {
            setters[i] = UniVarFuncs[popInt()];
        }
        shader->setters = setters;

        /* shader count */
        uint8 stageCount = (uint8) popInt();
        shader->stageCount = stageCount;

        /* shaders */
        shader->stages = (const char **) malloc(sizeof (const char *)*stageCount);

        for (uint8 i = 0; i < stageCount; i++) {

            /* i-th shader */
            src = popString();

            dest = (char *) malloc(sizeof (char) * (strlen(src) + 1));
            shader->stages[i] = (const char *) strcpy(dest, src);
            info("Shader: %s", shader->stages[i]);
        }

        mesh->draw = drawArrays;
        mesh->mode = DrawMode[vaoType];

        object->mesh = mesh;
        object->state = newState();
        object->shader = shader;
        object->mats = mat;

        objects[j] = object;
    }

    termScript();

    renderer->objectCount = objectCount;
    renderer->objects = objects;
}

void reloadScene(Engine *renderer) {
    return_guard(renderer, RVOID);

    info("%s", "# # # # # Reloading scene # # # # #");
    freeObjects(renderer);
    clearCache(renderer->shaderCache);
    clearCache(renderer->uniformCache);
    deallocStores();

    loadScene(renderer);
    preloadObjects(renderer);
    info("%s", "######### Scene reloaded #########");
}
