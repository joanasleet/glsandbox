#include "common.h"

#include "Util.h"
#include "Material.h"
#include "LuaScript.h"
#include "SceneManager.h"
#include "LookupManager.h"

void loadScene(Engine *renderer) {
    return_guard(renderer, RVOID);

    log_info("%s", "- - - - - Loading Scene - - - - -");

    /* get scene script name */
    script *config;
    lua( config, CONFIG_SCRIPT );
    pushTableKey( config, "engineConfig", "sceneScript" );
    char* scene;
    popString( config, scene );
    lua_close( config );

    /* load scene */
    script *S;
    lua( S, (const char *) scene );

    /* push scene table */
    lua_getglobal( S, "scene" );

    /* push subtable */
    lua_getfield( S, -1, "objects" );

    /* get subtable size */
    lua_len( S, -1 );
    uint32 objectCount = 0;
    popInt( S, objectCount );
    log_info("<Object count %d>", objectCount);
    return_guard(objectCount, RVOID);

    Object **objects = alloc( Object*, objectCount );

    /* traverse objects table */
    for( int j = 0; j < objectCount; ++j ) {

        Object *object = newObject();
        Mesh *mesh = newMesh();

        /* push object[j] */
        lua_geti( S, -1, j+1 );

        /* object name */
        lua_getfield( S, -1, "name" );
        char *name;
        popString( S, name );
        log_info( "<Adding object %s>", name );

        /* push mesh */
        lua_getfield( S, -1, "mesh" );

        lua_getfield( S, -1, "type" );
        uint32 vaoType;
        popInt( S, vaoType );

        lua_getfield( S, -1, "size" );
        float size;
        popFloat( S, size );

        lua_getfield( S, -1, "texres" );
        float texres;
        popFloat( S, texres );

        lua_getfield( S, -1, "position" );

        /* position[0] */
        lua_geti( S, -1, 1 );
        float midX;
        popFloat( S, midX );

        /* position[1] */
        lua_geti( S, -1, 2 );
        float midY;
        popFloat( S, midY );

        /* position[2] */
        lua_geti( S, -1, 3 );
        float midZ;
        popFloat( S, midZ );

        lua_pop( S, 2 );

        VaoFuncs[vaoType]( size, texres, midX, midY, midZ, mesh );

        /* push material */
        lua_getfield( S, -1, "material" );

        lua_len( S, -1 );
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
            mat->textures = alloc( Texture*, texCount );

            for (int32 i = 0; i < texCount; ++i) {

                /* get texture[1]...[texCount] */
                lua_geti( S, -1, i+1 );
                char *tex;
                popString( S, tex );
                mat->textures[i] = newTex2D( tex );
            }
        }
        break;
        case 6: {
            const char *faces[6];
            
            /* material[1]...[6] */
            lua_geti( S, -1, 1 );
            char *str1;
            popString( S, str1 );
            faces[0] = str1;

            lua_geti( S, -1, 2 );
            char *str2;
            popString( S, str2 );
            faces[1] = str2;

            lua_geti( S, -1, 3 );
            char *str3;
            popString( S, str3 );
            faces[2] = str3;

            lua_geti( S, -1, 4 );
            char *str4;
            popString( S, str4 );
            faces[3] = str4;

            lua_geti( S, -1, 5 );
            char *str5;
            popString( S, str5 );
            faces[4] = str5;

            lua_geti( S, -1, 6 );
            char *str6;
            popString( S, str6 );
            faces[5] = str6;

            mat->texCount = 1;
            mat->textures = alloc( Texture*, 1 );
            mat->textures[0] = cubeTexture((const char **) faces, 0, 0);
        }
        break;
        default:
            break;
        }
        lua_pop( S, 1 );

        /* push uniform map */
        lua_getfield( S, -1, "uniformsMap" );

        /* count uniformsMap entries */
        uint8 uniformCount = 0;
        lua_pushnil( S );
        while( lua_next( S, -2 ) != 0 ) {
            uniformCount++;
            lua_pop( S, 1 );
        }

        Shader *shader = newShader();
        shader->uniformCount = uniformCount;
        shader->uniforms = alloc( char*, uniformCount );
        shader->setters = alloc( UniformSetter*, uniformCount );

        /* read uniforms map */
        int i = 0;
        lua_pushnil( S );
        while( lua_next( S, -2 ) != 0 ) {

            /* uniform setter */
            int type = -1;
            popInt( S, type );
            shader->setters[i] = UniVarFuncs[type];
            
            /* pop value, leave key */

            /* uniform name */
            char *univar;
            getStringAlloc( S, univar );
            shader->uniforms[i] = univar;

            i++;
        }
        lua_pop( S, 1 );

        /* push shaders subtable */
        lua_getfield( S, -1, "shaders" );

        /* 
         * shader stages */
        lua_len( S, -1 );
        uint8 stageCount = 0;
        popInt( S, stageCount );
        shader->stageCount = stageCount;
        shader->stages = alloc( char*, stageCount);

        /* read table values */
        for( uint8 i = 0; i < stageCount; i++ ) {

            /* i-th shader stage */
            lua_geti( S, -1, i+1 );
            char *stage;
            popStringAlloc( S, stage );
            shader->stages[i] = stage;
        }
        lua_pop( S, 1 );

        mesh->draw = drawArrays;
        mesh->mode = DrawMode[vaoType];

        object->mesh = mesh;
        object->state = newState();
        object->shader = shader;
        object->mats = mat;

        objects[j] = object;

        /* pop objects[j] */
        lua_pop( S, 1 );
    }

    lua_close( S );

    renderer->objectCount = objectCount;
    renderer->objects = objects;
}

void reloadScene(Engine *renderer) {
    return_guard(renderer, RVOID);

    log_info("%s", "- - - - - Reloading Scene - - - - -");
    freeObjects(renderer);
    lua_close(renderer->shaderCache);
    lua_close(renderer->uniformCache);

    loadScene(renderer);
    preloadObjects(renderer);
    log_info("%s", "- - - - - Done Reloading - - - - -");
}

