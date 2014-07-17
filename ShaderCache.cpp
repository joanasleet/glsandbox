#include "ShaderCache.h"

Hash* newCache() {
    Hash* cache = (Hash*) malloc(sizeof (Hash));

    return cache;
}

GLuint get(Hash* cache, const char* key) {

    return 0;
}

void cache(Hash* cache, const char* key, GLint value) {
    printf("Caching shader %s (Id: %i)\n", key, value);
}

char* getKey(char* str, GLint num) {
    int len = strlen((const char*) str);
    int digits = floor(log10(abs(num))) + 1;
    size_t size = len + digits + 2;
    char* key = (char*) malloc(sizeof (char)*size);
    sprintf(key, "%i_%s", num, str);
    return key;
}