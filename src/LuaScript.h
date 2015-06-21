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
    err_guard( !lua_isnil( script, -1 ) && lua_isnumber( script, -1 ) ); \
    target = lua_tointeger( script, -1 ); \
    lua_pop( script, 1 ); \

#define popFloat( script, target ) \
    err_guard( !lua_isnil( script, -1 ) && lua_isnumber( script, -1 ) ); \
    target = lua_tonumber( script, -1 ); \
    lua_pop( script, 1 ); \

#define popString( script, target ) \
    err_guard( !lua_isnil( script, -1 ) && lua_isstring( script, -1 ) ); \
    size_t target##len; \
    const char* target##val = lua_tolstring( script, -1, &target##len ); \
    char target##temp[target##len+1]; \
    strcpy( target##temp, target##val ); \
    target = target##temp; \
    lua_pop( script, 1 ); \

#define pushTable( script, table ) \
    lua_getglobal( script, table ); \

#define pushKey( script, key ) \
    lua_getfield( script, -1, key ); \

#define pushTableKey( script, table, key ) \
    lua_getglobal( script, table ); \
    lua_getfield( script, -1, key ); \
    lua_remove( script, -2 ); \

// lua state with a table at the stacks top
typedef lua_State table;

// expects table at index -2
#define putKeyInt( table, key, num ) \
    lua_pushinteger( table, luaL_checkinteger( table, num ) ); \
    lua_setfield( table, -2, key ); \

// expects table at index -2
#define putKeyString( table , key, str ) \
    lua_pushstring( table, luaL_checkstring( table, str ) ); \
    lua_setfield( table, -2, key ); \

#define popVal( script, num ) if( script ) lua_pop( script, num )

#define freeScript( script ) if( script ) lua_close( script )

#endif
