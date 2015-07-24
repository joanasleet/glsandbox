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
    glClearColor(0.2, 0.2, 0.2, 1.0);

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
    
    renderer->context = newContext( xRes, yRes, (const char*) title );

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

void render(Object *obj, Engine *renderer, float alpha ) {

    /*
     * interpolate camera */
    Camera *cam = renderer->mainCam;

    /* save old state */
    State *state = cam->state;

    /* setup interpolated state */
    State interpolatedState;

    /* setup position */
    float position[3];
    calcPosition( position, state, alpha );
    cpyBuf( interpolatedState.position, position, 3 );

    /* setup orientation */
    float orientation[4];
    calcOrientation( orientation, state, alpha);
    cpyBuf( interpolatedState.orientation, orientation, 4 );

    /* set interpolated state */
    cam->state = &interpolatedState;

    /*
     * update uniforms */
    Shader *shader = obj->shader;
    GLint program = shader->program;
    glUseProgram(program);

    double globalTime = getGlobalTime();

    //log_info( "%s", luaL_typename( shader->uniforms, -1 ) );

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
    //log_info( "> %s", luaL_typename( shader->uniforms, -1 ) );

    /*
     * reset old state */
    cam->state = state;

    /*
     * bind materials */
    Material *mat = obj->mats;

    Texture *tex;
    for (uint32 i = 0; i < mat->texCount; ++i) {

        tex = mat->textures[i];
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(tex->target, tex->id);
    }

    /*
     * draw object */
    Mesh *mesh = obj->mesh;
    glBindVertexArray(mesh->vaoId);
    mesh->draw(mesh->mode, &mesh->first, mesh->count);
}

void enterLoop(Engine *renderer) {

    exit_guard(renderer);

    Camera *cam = renderer->mainCam;
    GLFWwindow *window = renderer->context->win;

    double dt, lag = 0.0;
    double updateInterval = 1.0/60.0;

    startTimer();

    log_info("%s", "- - - - - - - Rendering - - - - - - -");
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {

        dt = elapsedTime();
        lag += dt;

        fps(dt);

        while ( lag >= updateInterval ) {

            updateCam(cam);
            lag -= updateInterval;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* render objects */
        float renderAlpha = (float) (lag / updateInterval);
        for (unsigned int i = 0; i < renderer->objectCount; ++i) {
            render( renderer->objects[i], renderer, renderAlpha );
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void terminate(Engine *renderer) {

    lua_close( renderer->shaderCache );
    lua_close( renderer->textureCache );

    freeObjects(renderer);

    free(renderer->context);

    freeCamera(renderer->mainCam);

    free(renderer);

    glfwTerminate();
}

void freeObjects(Engine *renderer) {

    Object *freeMe;
    for (uint32 i = 0; i < renderer->objectCount; ++i) {
        freeMe = renderer->objects[i];
        freeObject(freeMe);
        renderer->objects[i] = NULL;
    }
    free(renderer->objects);
    renderer->objects = NULL;
}
