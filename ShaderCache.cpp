#include "ShaderCache.h"

Element* newElement(const char* key, GLint value) {
    Element* element = (Element*) malloc(sizeof (Element));
    if (!element) {
        ERR("Failed to allocate Element");
        return NULL;
    }
    element->key = key;
    element->value = value;

    element->next = NULL;
    element->prev = NULL;

    return element;
}

Hash* newCache() {
    Hash* cache = (Hash*) malloc(sizeof (Hash));
    if (!cache) {
        ERR("Failed to allocate Cache");
        return NULL;
    }

    for (int i = 0; i < BUCKETS; ++i) {
        cache->buckets[i].last = NULL;
    }

    return cache;
}

GLint get(Hash* cache, const char* key) {

    int index = hash(key);

    Bucket target = cache->buckets[index];

    Element* iterator = target.last;

    if (!iterator) {
        INFO("Key %s is not cached.", key);
        return NOT_CACHED;
    }

    do {
        if (strcmp(iterator->key, key) == 0) {
            INFO("Retrieved value %i (key %s)\n", iterator->value, key);
            return iterator->value;
        }
        iterator = iterator->prev;
    } while (iterator);

    INFO("Key %s is not cached.", key);
    return NOT_CACHED;
}

void cache(Hash* cache, const char* key, GLint value) {
    INFO("Caching key %s (value %i)\n", key, value);

    int index = hash(key);
    Bucket target = cache->buckets[index];

    Element* last = target.last;
    Element* newE = newElement(key, value);

    if (last) {
        last->next = newE;
        newE->prev = last;
    }

    cache->buckets[index].last = newE;
    
}

int hash(const char* key) {

    return 0;
}

char* getKey(char* str, GLint num) {
    int len = strlen((const char*) str);
    int digits = floor(log10(abs(num))) + 1;
    size_t size = len + digits + 2;
    char* key = (char*) malloc(sizeof (char)*size);
    sprintf(key, "%i_%s", num, str);
    return key;
}

void printCache(Hash* cache) {

}