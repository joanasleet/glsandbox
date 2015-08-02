#include "common.h"

#include "Util.h"
#include "Material.h"
#include "SceneManager.h"
#include "LookupManager.h"

void loadObjects( Engine* renderer, script *S ) {

    /* push scene table */
    lua_getglobal( S, "scene" );

    /* push target table */
    lua_getfield( S, -1, "objects" );

    /* return if no objects table */
    if( lua_isnil( S, -1 ) ) {
        lua_pop( S, 2 );
        return;
    }

    /* get subtable size */
    lua_len( S, -1 );
    popInt( S, renderer->objCount );
    uint32 objectCount = renderer->objCount;
    log_info("<Objects: %d >", objectCount );
    return_guard(objectCount, RVOID);

    /* objects */
    renderer->objects = alloc( Object*, objectCount ); 
    Object **objects = renderer->objects;
    for( int j = 0; j < objectCount; ++j ) {

        Object *object = ( objects[j] = newObject() );
        object->state = newState();
        Mesh *mesh = ( object->mesh = newMesh() );

        /* push object[j] */
        lua_geti( S, -1, j+1 );

        /* object name */
        lua_getfield( S, -1, "name" );
        char *name;
        popString( S, name );
        log_info( "<Object: %s >", name );

        /* push mesh */
        lua_getfield( S, -1, "mesh" );

        lua_getfield( S, -1, "type" );
        uint32 vaoType;
        popInt( S, vaoType );
        mesh->draw = DrawFuncs[vaoType];
        mesh->mode = DrawMode[vaoType];
        log_info( "<Type: %d>", vaoType );

        lua_getfield( S, -1, "size" );
        float size;
        popFloat( S, size );
        log_info( "<Size: %.1f>", size );

        /*
         * create da physics */

        lua_getfield( S, -1, "physics" );
        int physics;
        popInt( S, physics );
        log_info( "<Physics: %d>", physics );

        lua_getfield( S, -1, "texres" );
        float texres;
        popFloat( S, texres );
        log_info( "<TexRes: %.1f>", texres );

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

        object->state->position[0] = midX;
        object->state->position[1] = midY;
        object->state->position[2] = midZ;

        log_info( "<Position: ( %.1f, %.1f, %.1f )>", midX, midY, midZ );

        lua_pop( S, 2 );

        VaoFuncs[vaoType]( size, texres, mesh );

        /* physics properties */
        float transform[16];
        mat4trans( transform, object->state->position );
        mesh->nbody = PhysicsBodyFuncs[physics]( renderer->nworld, mesh->ncol, transform );
        NewtonBodySetMassMatrix( mesh->nbody, size, size, size, size );
        //NewtonBodySetMassProperties( mesh->nbody, size, mesh->ncol );
        NewtonBodySetCollidable( mesh->nbody, 1 );
        NewtonBodySetLinearDamping( mesh->nbody, 0.0f );

        NewtonBodySetForceAndTorqueCallback( mesh->nbody, applyForceAndTorque );
        NewtonBodySetTransformCallback( mesh->nbody, onTransform );
        NewtonBodySetUserData( mesh->nbody, object->state );

        /* push material */
        lua_getfield( S, -1, "material" );

        lua_len( S, -1 );
        int32 texCount;
        popInt( S, texCount );

        Material *mat = ( object->mats = newMaterial() );
        log_info( "<Material: %d >", texCount );

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
                log_info( "< %s >", tex );
                mat->textures[i] = fileTexture( tex, GL_TEXTURE_2D, TRUE );
            }
        }
        break;
        case 6: {
            const char *faces[6];
            
            /* material[1]...[6] */
            lua_geti( S, -1, 1 );
            char *str1;
            popString( S, str1 );
            log_info( "< %s >", str1 );
            faces[0] = str1;

            lua_geti( S, -1, 2 );
            char *str2;
            popString( S, str2 );
            log_info( "< %s >", str2 );
            faces[1] = str2;

            lua_geti( S, -1, 3 );
            char *str3;
            popString( S, str3 );
            log_info( "< %s >", str3 );
            faces[2] = str3;

            lua_geti( S, -1, 4 );
            char *str4;
            popString( S, str4 );
            log_info( "< %s >", str4 );
            faces[3] = str4;

            lua_geti( S, -1, 5 );
            char *str5;
            popString( S, str5 );
            log_info( "< %s >", str5 );
            faces[4] = str5;

            lua_geti( S, -1, 6 );
            char *str6;
            popString( S, str6 );
            log_info( "< %s >", str6 );
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

        /*
         * build shader */
        Shader *shader = ( object->shader = newShader() );

        /* push shaders subtable */
        lua_getfield( S, -1, "shaders" );

        /* shader stages */
        int i = 0;
        lua_pushnil( S );
        log_info( "<Shader ( prog: %d )>", shader->program );
        while( lua_next( S, -2 ) != 0 ) {

            /* value = shader stage */
            char *stage;
            popString( S, stage );
            log_info( "<%s: %s>", ShaderName[i], stage );
            addShader( stage, ShaderType[i++], shader->program, renderer->shaderCache );

            /* ignore key */
        }
        lua_pop( S, 1 );

        /* push uniform map */
        lua_getfield( S, -1, "uniformsMap" );

        /* read uniforms map */
        lua_pushnil( S );
        while( lua_next( S, -2 ) != 0 ) {

            /* uniform setter */
            int type = -1;
            popInt( S, type );
            
            /* pop value, leave key */

            /* uniform name */
            char *univar;
            getString( S, univar );

            /* uniforms[ location(univar) ] = type */
            lua_pushinteger( shader->uniforms, type );
            GLint loc = glGetUniformLocation( shader->program, ( const char* ) univar );
            if (loc < 0) {
                log_warn("No uniform '%s' in shader prog %i", univar, shader->program );
                lua_pop( shader->uniforms, 1 );
            } else { 
                lua_seti( shader->uniforms, -2, loc );
            }

            log_info( "<%s ( loc = %d ): %d>", univar, loc, type );
        }
        lua_pop( S, 1 );

        /* pop objects[j] */
        lua_pop( S, 1 );
    }
    /* pop objects and scene table */
    lua_pop( S, 2 );
}

void loadTextures( Engine* renderer, script *S ) {

    /* push scene table */
    lua_getglobal( S, "scene" );

    /* push target table */
    lua_getfield( S, -1, "textures" );

    /* return if no textures table */
    if( lua_isnil( S, -1 ) ) {
        lua_pop( S, 2 );
        return;
    }

    /* get subtable size */
    lua_len( S, -1 );
    popInt( S, renderer->texCount );
    uint32 objectCount = renderer->texCount;
    log_info("<Textures: %d >", objectCount );
    return_guard(objectCount, RVOID);

    /* objects */
    renderer->textures = alloc( Object*, objectCount );
    Object **objects = renderer->textures;
    for( uint32 j = 0; j < objectCount; ++j ) {

        Object *object = ( objects[j] = newObject() );
        object->state = newState();
        Mesh *mesh = ( object->mesh = newMesh() );

        /* push object[j] */
        lua_geti( S, -1, j+1 );

        /* object name */
        lua_getfield( S, -1, "name" );
        char *name;
        popString( S, name );
        log_info( "<Object: %s >", name );

        /* store texture: textureCache[name] = tex */
        Texture *tex = emptyTexture( renderer->context->xRes, renderer->context->yRes, GL_TEXTURE_2D, FALSE );
        lua_pushlightuserdata( renderer->textureCache, (void*) tex );
        lua_setfield( renderer->textureCache, -2, (const char*) name );

        /* create framebuffer */
        glGenFramebuffers( 1, &( mesh->fboId ) );
        glBindFramebuffer( GL_FRAMEBUFFER, mesh->fboId );

        /* attach texture to fbo */
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0 );

        /* check fbo completeness */
        err_guard( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE );

        /* push mesh */
        lua_getfield( S, -1, "mesh" );

        lua_getfield( S, -1, "type" );
        uint32 vaoType;
        popInt( S, vaoType );
        mesh->draw = DrawFuncs[vaoType];
        mesh->mode = DrawMode[vaoType];
        log_info( "<Type: %d>", vaoType );

        lua_getfield( S, -1, "size" );
        float size;
        popFloat( S, size );
        log_info( "<Size: %.1f>", size );

        lua_getfield( S, -1, "texres" );
        float texres;
        popFloat( S, texres );
        log_info( "<TexRes: %.1f>", texres );

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
        log_info( "<Position: ( %.1f, %.1f, %.1f )>", midX, midY, midZ );

        lua_pop( S, 2 );

        VaoFuncs[vaoType]( size, texres, mesh );

        /* push material */
        lua_getfield( S, -1, "material" );

        lua_len( S, -1 );
        int32 texCount;
        popInt( S, texCount );

        Material *mat = ( object->mats = newMaterial() );
        log_info( "<Material: %d >", texCount );

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
                char *texName;
                popString( S, texName);
                log_info( "< %s >", texName );
                mat->textures[i] = fileTexture( texName, GL_TEXTURE_2D, TRUE );
            }
        }
        break;
        case 6: {
            const char *faces[6];
            
            /* material[1]...[6] */
            lua_geti( S, -1, 1 );
            char *str1;
            popString( S, str1 );
            log_info( "< %s >", str1 );
            faces[0] = str1;

            lua_geti( S, -1, 2 );
            char *str2;
            popString( S, str2 );
            log_info( "< %s >", str2 );
            faces[1] = str2;

            lua_geti( S, -1, 3 );
            char *str3;
            popString( S, str3 );
            log_info( "< %s >", str3 );
            faces[2] = str3;

            lua_geti( S, -1, 4 );
            char *str4;
            popString( S, str4 );
            log_info( "< %s >", str4 );
            faces[3] = str4;

            lua_geti( S, -1, 5 );
            char *str5;
            popString( S, str5 );
            log_info( "< %s >", str5 );
            faces[4] = str5;

            lua_geti( S, -1, 6 );
            char *str6;
            popString( S, str6 );
            log_info( "< %s >", str6 );
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

        /*
         * build shader */
        Shader *shader = ( object->shader = newShader() );

        /* push shaders subtable */
        lua_getfield( S, -1, "shaders" );

        /* shader stages */
        int i = 0;
        lua_pushnil( S );
        log_info( "<Shader ( prog: %d )>", shader->program );
        while( lua_next( S, -2 ) != 0 ) {

            /* value = shader stage */
            char *stage;
            popString( S, stage );
            log_info( "<%s: %s>", ShaderName[i], stage );
            addShader( stage, ShaderType[i++], shader->program, renderer->shaderCache );

            /* ignore key */
        }
        lua_pop( S, 1 );

        /* push uniform map */
        lua_getfield( S, -1, "uniformsMap" );

        /* read uniforms map */
        lua_pushnil( S );
        while( lua_next( S, -2 ) != 0 ) {

            /* uniform setter */
            int type = -1;
            popInt( S, type );
            
            /* pop value, leave key */

            /* uniform name */
            char *univar;
            getString( S, univar );

            /* uniforms[ location(univar) ] = type */
            lua_pushinteger( shader->uniforms, type );
            GLint loc = glGetUniformLocation( shader->program, ( const char* ) univar );
            if (loc < 0) {
                log_warn("No uniform '%s' in shader prog %i", univar, shader->program );
                lua_pop( shader->uniforms, 1 );
            } else { 
                lua_seti( shader->uniforms, -2, loc );
            }

            log_info( "<%s ( loc = %d ): %d>", univar, loc, type );
        }
        lua_pop( S, 1 );

        /* pop objects[j] */
        lua_pop( S, 1 );
    }
    /* pop textures and scene table */
    lua_pop( S, 2 );
}

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

    log_info("%s", "- - - - loading textures - - - -");
    loadTextures( renderer, S );
    log_info("%s", "- - - - loading objects - - - -");
    loadObjects( renderer, S );

    lua_close( S );
}

void reloadScene(Engine *renderer) {
    return_guard(renderer, RVOID);

    log_info("%s", "- - - - - Reloading Scene - - - - -");
    freeTextures( renderer );
    freeObjects( renderer );

    NewtonDestroyAllBodies( renderer->nworld );

    clearTable( renderer->shaderCache );
    //clearTable( renderer->textureCache );

    loadScene(renderer);
    log_info("%s", "- - - - - Done Reloading - - - - -");
}

