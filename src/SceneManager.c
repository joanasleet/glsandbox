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

    log_info("%s", "- - - - - Loading Scene - - - - -");
    execScript(SCENE_LOADER);

    // camera
    Camera *cam = renderer->mainCam;

    cam->fov = popFloat();
    cam->targetFov = cam->fov;
    cam->aspectRatio = popFloat();
    cam->state->position[0] = popFloat();
    cam->state->position[1] = popFloat();
    cam->state->position[2] = popFloat();

    uint32 objectCount = (uint32) popInt();

    log_info("<Object count %d>", objectCount);
    return_guard(objectCount, RVOID);

    // objects
    Object **objects = (Object **) malloc(sizeof (Object *) * objectCount);

    for (uint32 j = 0; j < objectCount; j++) {

        Object *object = newObject();
        Mesh *mesh = newMesh();

        /* object name */
        log_info("<Adding object %s>", popString());

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
        case 2:
        case 3:
        case 4:
        case 5: {
            mat->texCount = (uint32) texCount;
            mat->textures = (Texture **) malloc(texCount * sizeof(Texture *));

            for (int32 i = 0; i < texCount; ++i) {
                mat->textures[i] = newTex2D(popString());
            }
        }
        break;
        case 6: {
            const char *faces[6];
            faces[0] = popString();
            faces[1] = popString();
            faces[2] = popString();
            faces[3] = popString();
            faces[4] = popString();
            faces[5] = popString();

            mat->texCount = 1;
            mat->textures = (Texture **) malloc(sizeof(Texture *));
            mat->textures[0] = cubeTexture((const char **) faces, 0, 0);
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
            log_info("<Uniform %s>", shader->uniforms[i]);
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

            /* i-th shader stage */
            src = popString();

            dest = (char *) malloc(sizeof (char) * (strlen(src) + 1));
            shader->stages[i] = (const char *) strcpy(dest, src);
            log_info("<Shader %s>", shader->stages[i]);
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

    log_info("%s", "- - - - - Reloading Scene - - - - -");
    freeObjects(renderer);
    clearCache(renderer->shaderCache);
    clearCache(renderer->uniformCache);
    deallocStores();

    loadScene(renderer);
    preloadObjects(renderer);
    log_info("%s", "- - - - - - - - - - - - - - - - - -");
}
