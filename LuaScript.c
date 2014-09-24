#include "Script.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

lua_State *mainRef;

void initScript(const char* script) {
    mainRef = luaL_newstate();
    luaL_openlibs(mainRef);

    if (luaL_loadfile(mainRef, script)) {
        fprintf(stderr, "Error loading script %s:\n%s", script, lua_tostring(mainRef, -1));
        termScript();
        exit(EXIT_FAILURE);
    }
}

void argScript(char argType, void* value) {
    switch (argType) {
        case ARG_NUMBER:
            printf("Passing Number %f\n", *(double*) value);
            break;
        case ARG_STRING:
            printf("Passing String %s\n", (char*) value);
            break;
        case ARG_TABLE:
            break;
        default:
            printf("Unknown arg type %d\n", argType);
            break;
    }
}

void exeScript() {
    if (lua_pcall(mainRef, 0, 0, 0)) {
        fprintf(stderr, "Error calling script:\n%s", lua_tostring(mainRef, -1));
        termScript();
        exit(EXIT_FAILURE);
    }
}

void termScript() {
    lua_close(mainRef);
}