#include "common.h"

//#include "Script.h"
#include "LuaScript.h"
#include "MeshUtil.h"
#include "Debugger.h"
#include "Material.h"
#include "SceneManager.h"
#include "LookupManager.h"

#define SCENE_LOADER "scripts/loadScene.lua"

void loadScene(Engine *renderer) {
    return_guard(renderer, RVOID);

    log_info("%s", "- - - - - Loading Scene - - - - -");
    script *S;
    lua( S, SCENE_LOADER );

    // camera
    Camera *cam = renderer->mainCam;

    popFloat( S, cam->fov );
    cam->targetFov = cam->fov;
    popFloat( S, cam->aspectRatio );
    popFloat( S, cam->state->position[0] );
    popFloat( S, cam->state->position[1] );
    popFloat( S, cam->state->position[2] );

    uint32 objectCount = 0;
    popInt( S, objectCount );

    log_info("<Object count %d>", objectCount);
    return_guard(objectCount, RVOID);

    // objects
    Object **objects = (Object **) malloc(sizeof (Object *) * objectCount);

    for (uint32 j = 0; j < objectCount; j++) {

        Object *object = newObject();
        Mesh *mesh = newMesh();

        /* object name */
        char * name;
        popString( S, name );
        log_info( "<Adding object %s>", name );
        free( name );

        /* vao type */
        uint32 vaoType;
        popInt( S, vaoType );

        float size;
        popFloat( S, size );

        float texres;
        popFloat( S, texres );

        float midX;
        popFloat( S, midX );

        float midY;
        popFloat( S, midY );

        float midZ;
        popFloat( S, midZ );

        mesh->vaoId = genVao(vaoType, size, texres, midX, midY, midZ, &(mesh->count));

        /* texture */
        int32 texCount;
        popInt( S, texCount );

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
                char *tex;
                popString( S, tex );
                mat->textures[i] = newTex2D( tex );
                free( tex );
            }
        }
        break;
        case 6: {
            const char *faces[6];
            //faces[0] = popString();
            //faces[1] = popString();
            //faces[2] = popString();
            //faces[3] = popString();
            //faces[4] = popString();
            //faces[5] = popString();

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
        uint8 uniformCount;
        popInt( S, uniformCount );
        shader->uniformCount = uniformCount;

        /* uniforms */
        shader->uniforms = (const char **) malloc(sizeof (const char *)*uniformCount);

        for (uint8 i = 0; i < uniformCount; i++) {

            /* i-th uniform variable */
            char *univar;
            popString( S, univar );

            shader->uniforms[i] = (const char *) univar;
            log_info("<Uniform %s>", shader->uniforms[i]);
        }

        /* uniforms setter functions */
        UniformSetter *setters = (UniformSetter *) malloc(sizeof (UniformSetter) * uniformCount);
        for (uint8 i = 0; i < uniformCount; i++) {
            int indx = 0;
            popInt( S, indx );
            setters[i] = UniVarFuncs[indx];
        }
        shader->setters = setters;

        /* shader count */
        uint8 stageCount = 0;
        popInt( S, stageCount );
        shader->stageCount = stageCount;

        /* shaders */
        shader->stages = (const char **) malloc(sizeof (const char *)*stageCount);

        for (uint8 i = 0; i < stageCount; i++) {

            /* i-th shader stage */
            char *stage;
            popString( S, stage );

            shader->stages[i] = (const char *) stage;
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

    freeScript( S );

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

