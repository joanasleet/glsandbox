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
        ERR("Key '%s' is not cached.", key);
        return NOT_CACHED;
    }

    do {
        if (strcmp(iterator->key, key) == 0) {
            INFO("Retrieved value %i (key '%s')", iterator->value, key);
            return iterator->value;
        }
        iterator = iterator->prev;
    } while (iterator);

    ERR("Key '%'s is not cached.", key);
    return NOT_CACHED;
}

void cache(Hash* cache, const char* key, GLint value) {
    INFO("Caching key '%s' (value: %i)", key, value);

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

// djb2

int hash(const char* key) {

    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % BUCKETS;
}

const char* getKey(const char* str, GLint num) {
    int len = strlen(str);
    int digits = floor(log10(abs(num))) + 1;
    size_t size = len + digits + 2;
    char* key = (char*) malloc(sizeof(char)*size);
    sprintf(key, "%i_%s", num, str);
    const char* rkey = (const char*) key;
    return rkey;
}

void printCache(Hash* cache) {

    Bucket bit;
    Element* elit;

    for (int i = 0; i < BUCKETS; ++i) {
        bit = cache->buckets[i];
        elit = bit.last;

        printf("[%d] { ", i);

        if (!elit) {
            printf("}\n");
            continue;
        }

        do {
            printf("%s ", elit->key);
            elit = elit->prev;
        } while (elit);

        printf("}\n");
    }
}