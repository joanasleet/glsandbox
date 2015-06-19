#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "common.h"
#include "Debugger.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef lua_State script;

#define lua( state, path )   \
    state = luaL_newstate(); \
    err_guard( state );      \
    luaL_openlibs( state );  \
    if( luaL_dofile( state, path ) ) { \
        err( "[SCRIPT] %s: %s\n", path, lua_tostring( state, -1 ) ); \
        lua_close( state ); \
    } \

#define popInt( script, target ) \
    if( script && !lua_isnil( script, -1 ) && lua_isnumber( script, -1 ) ) { \
        int num = lua_tointeger( script, -1 ); \
        lua_pop( script, 1 ); \
        target = num; \
    } \

#define popFloat( script, target ) \
    if(  script && !lua_isnil( script, -1 ) && lua_isnumber( script, -1 ) ) { \
        double num = lua_tonumber( script, -1 ); \
        lua_pop( script, 1 ); \
        target = num; \
    } \

#define popString( script, target ) \
    if( script && !lua_isnil( script, -1 ) && lua_isstring( script, -1 ) ) { \
        size_t _strlen; \
        const char* _src = lua_tolstring( script, -1, &_strlen ); \
        target = malloc( sizeof( char ) * (_strlen+1) ); \
        err_guard( target ); \
        strcpy( target, _src ); \
        lua_pop( script, 1 ); \
    } \

#define pushKey( script, table, key ) \
    if( script ) { \
        lua_getglobal( script, table ); \
        lua_getfield( script, -1, key ); \
        lua_remove( script, -2 ); \
    } \

#define popVal( script, num ) if( script ) lua_pop( script, num )

#define freeScript( script ) if( script ) lua_close( script )

#endif
