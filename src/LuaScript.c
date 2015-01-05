#include "Script.h"
#include "Debugger.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static lua_State *L;
static const char *currentScript;

void execScript(const char *script) {

    L = luaL_newstate();
    err_guard(L);

    currentScript = script;

    luaL_openlibs(L);

    if (luaL_loadfile(L, script)) {
        err("[SCRIPT] %s:\n%s", script, lua_tostring(L, -1));
        termScript();
    }

    if (lua_pcall(L, 0, LUA_MULTRET, 0)) {
        err("[SCRIPT] %s:\n%s", script, lua_tostring(L, -1));
        termScript();
    }
}

// TODO: finish
void argScript(uint8 argType, void *value) {
    switch (argType) {
    case ARG_NUMBER:
        printf("Passing Number %f\n", *(double *) value);
        break;
    case ARG_STRING:
        printf("Passing String %s\n", (char *) value);
        break;
    case ARG_TABLE:
        break;
    default:
        printf("Unknown arg type %d\n", argType);
        break;
    }
}

void dumpResult() {

    while (!lua_isnil(L, -1)) {

        if (lua_isnumber(L, -1)) {
            printf("Got number from script: %ld\n", lua_tointeger(L, -1));
        } else if (lua_isstring(L, -1)) {
            printf("Got string from script: %s\n", lua_tostring(L, -1));
        }
        lua_pop(L, 1);
    }
}

int popInt() {

    if (!lua_isnil(L, -1) && lua_isnumber(L, -1)) {
        int number = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return number;
    }

    warn("[SCRIPT] %s: returning fallback value", currentScript);
    return -1;
}

double popFloat() {

    if (!lua_isnil(L, -1) && lua_isnumber(L, -1)) {
        double number = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return number;
    }

    warn("[SCRIPT] %s: returning fallback value", currentScript);
    return -1.0;
}

const char *popString() {

    if (!lua_isnil(L, -1) && lua_isstring(L, -1)) {
        const char *string = lua_tostring(L, -1);
        lua_pop(L, 1);
        return string;
    }

    warn("[SCRIPT] %s: returning fallback value", currentScript);
    return "fallback";
}

void termScript() {
    if (L) lua_close(L);
}