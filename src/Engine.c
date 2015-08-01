#include "Util.h"
#include "Engine.h"
#include "Timestep.h"
#include "LuaScript.h"
#include "InputManager.h"
#include "LookupManager.h"

#include <math.h>
#include <string.h>

Engine *init() {

    Engine *renderer = alloc( Engine, 1 );

    renderer->nworld = NewtonCreate();

    /* read config from script */
    config( renderer );

    /* lua table as shader cache */
    renderer->shaderCache = luaL_newstate();
    err_guard( renderer->shaderCache );
    lua_newtable( renderer->shaderCache );

    /* lua table as texture cache */
    renderer->textureCache = luaL_newstate();
    err_guard( renderer->textureCache );
    lua_newtable( renderer->textureCache );

    /* lua table as texture data cache */
    renderer->texDataCache = luaL_newstate();
    err_guard( renderer->texDataCache );
    lua_newtable( renderer->texDataCache );

    /* set object count */
    renderer->objects = NULL;
    renderer->textures = NULL;
    renderer->objCount = 0;
    renderer->texCount = 0;

    /* set window callbacks */
    GLFWwindow *window = renderer->context->win;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorEnterCallback(window, cursorEnterCB);
    glfwSetWindowSizeCallback(window, resizeCB);
    glfwSetScrollCallback(window, scrollCB);
    glfwSetKeyCallback(window, keyCB);

    /* set GL states */
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );

    return renderer;
}

void config( Engine *renderer ) {

    script *S;
    lua( S, CONFIG_SCRIPT );

    /* get table by name */
    lua_getglobal( S, "engineConfig" );

    /* context */
    int xRes;
    lua_getfield( S, -1, "resolutionX" );
    popInt( S, xRes );

    int yRes;
    lua_getfield( S, -1, "resolutionY" );
    popInt( S, yRes );

    char *title;
    lua_getfield( S, -1, "windowTitle" );
    popString( S, title );
    
    renderer->context = newContext( ( uint32 ) xRes, ( uint32 ) yRes, (const char*) title );

    /* camera */
    float posX;
    lua_getfield( S, -1, "posX" );
    popFloat( S, posX );

    float posY;
    lua_getfield( S, -1, "posY" );
    popFloat( S, posY );

    float posZ;
    lua_getfield( S, -1, "posZ" );
    popFloat( S, posZ );

    Camera* cam = newCamera( posX, posY, posZ );
    
    float fov;
    lua_getfield( S, -1, "fieldOfView" );
    popFloat( S, fov );
    cam->fov[0] = cam->fov[1] = defaultFov = fov;
    
    lua_getfield( S, -1, "acceleration" );
    popFloat( S, cam->state->accel );

    float sensitivity;
    lua_getfield( S, -1, "sensitivity" );
    popFloat( S, sensitivity );
    cam->sensitivity = sensitivity;

    float near;
    lua_getfield( S, -1, "nearClip" );
    popFloat( S, near );
    cam->nearClip = near;

    float far;
    lua_getfield( S, -1, "farClip" );
    popFloat( S, far );
    cam->farClip = far;

    lua_getfield( S, -1, "aspectRatio" );
    popFloat( S, cam->aspectRatio );
    
    float smoothing;
    lua_getfield( S, -1, "smoothing" );
    popFloat( S, smoothing );
    cam->smoothing = smoothing;

    renderer->mainCam = cam;

    /* config done */

    lua_close( S );
}

void genTextures( Engine *renderer ) {

    updateCam( renderer->mainCam );

    Object **textures = renderer->textures;
    for( uint32 i=0; i<renderer->texCount; ++i ) {

        Object *tex = textures[i];
        glBindFramebuffer( GL_FRAMEBUFFER, tex->mesh->fboId );

        render( tex, renderer, 1.0f );
    }
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void render(Object *obj, Engine *renderer, float alpha ) {

    Camera *cam = renderer->mainCam;

    /* old state */
    State *state = cam->state;

    /* interpolated state */
    State interpolatedState;

    /* position */
    calcPosition( interpolatedState.position, state, alpha );

    /* orientation */
    float nextOriState[4];
    quatSlerp( state->orientation, state->targetOrien, nextOriState, cam->smoothing );
    quatSlerp( state->orientation, nextOriState, interpolatedState.orientation, alpha );
    quatNorm( interpolatedState.orientation );

    /* set interpolated state */
    cam->state = &interpolatedState;

    /*
     * update uniforms */
    Shader *shader = obj->shader;
    GLuint program = shader->program;
    glUseProgram(program);

    double globalTime = getGlobalTime();

    lua_pushnil( shader->uniforms );
    while( lua_next( shader->uniforms, -2 ) != 0 ) {

        /* value = type */
        int type = -1;
        popInt( shader->uniforms, type );
        
        /* key = uniform location */
        GLint loc = -1;
        getInt( shader->uniforms, loc );

        // TODO: interpolate object state
        

        UniVarFuncs[type](loc, cam, obj->state, globalTime);
    }

    /* restore old state */
    cam->state = state;

    /* bind materials */
    Material *mat = obj->mats;
    for (uint32 i = 0; i < mat->texCount; ++i) {

        Texture *tex = mat->textures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(tex->target, tex->id);
    }

    /* draw object */
    Mesh *mesh = obj->mesh;
    glBindVertexArray(mesh->vaoId);
    mesh->draw(mesh->mode, &mesh->first, mesh->count);
}

void enterLoop(Engine *renderer) {

    exit_guard(renderer);

    Camera *cam = renderer->mainCam;
    GLFWwindow *window = renderer->context->win;

    float dt, lag = 0.0f;
    float timestep = 1.0f/60.0f;

    startTimer();

    log_info("%s", "- - - - - - - Rendering - - - - - - -");
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        dt = ( float ) elapsedTime();
        lag += dt;

        fps(dt);

        while ( lag >= timestep ) {

            updateCam(cam);
            NewtonUpdate( renderer->nworld, timestep );
            lag -= timestep;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* render objects */
        float renderAlpha = ( lag / timestep );
        for (unsigned int i = 0; i < renderer->objCount; ++i) {
            render( renderer->objects[i], renderer, renderAlpha );
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void terminate(Engine *renderer) {

    lua_close( renderer->shaderCache );
    lua_close( renderer->textureCache );
    lua_close( renderer->texDataCache );

    freeTextures( renderer );
    freeObjects( renderer );

    freeCamera( renderer->mainCam );

    free( renderer->context );

    free(renderer);

    glfwTerminate();
}

void freeTextures( Engine *renderer ) {

    Object *freeMe;
    for (uint32 i = 0; i < renderer->texCount; ++i) {
        freeMe = renderer->textures[i];
        freeObject(freeMe);
    }
    free( renderer->textures );
}

void freeObjects( Engine *renderer ) {

    Object *freeMe;
    for (uint32 i = 0; i < renderer->objCount; ++i) {
        freeMe = renderer->objects[i];
        freeObject(freeMe);
    }
    free( renderer->objects );
}

