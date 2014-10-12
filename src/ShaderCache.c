#include "ShaderCache.h"
#include "Debugger.h"
#include "Deallocator.h"
#include <string.h>

Element* newElement(const char* key, GLint value) {
    Element* element = (Element*) malloc(sizeof (Element));
    return_guard(element, NULL);
    /*if (!element) {
        err("Failed to allocate Element");
        return NULL;
    }*/
    element->key = key;
    element->value = value;

    element->next = NULL;
    element->prev = NULL;

    return element;
}

Cache* newCache() {
    Cache* cache = (Cache*) malloc(sizeof (Cache));

    return_guard(cache, NULL);
    /*if (!cache) {
        err("Failed to allocate Cache");
        return NULL;
    }*/

    for (int i = 0; i < BUCKETS; ++i) {
        cache->buckets[i].last = NULL;
    }

    return cache;
}

void freeCache(Cache* hash) {
    clearCache(hash);
    free(hash);
}

void clearCache(Cache* hash) {
    Bucket b;
    Element* it;
    Element* freeMe;
    for (int i = 0; i < BUCKETS; i++) {

        b = hash->buckets[i];
        it = b.last;

        while (it) {
            if (it->key) free((char*) it->key); // possible issue
            it->key = NULL;
            freeMe = it;
            it = it->prev;
            free(freeMe);
        }

        hash->buckets[i].last = NULL;
    }
}

GLint get(Cache* cache, const char* key) {

    int index = hash(key);

    Bucket target = cache->buckets[index];

    Element* iterator = target.last;

    if (!iterator) {
        info("Key: %s not cached", key);
        return NOT_CACHED;
    }

    do {
        if (strcmp(iterator->key, key) == 0) {
            return iterator->value;
        }
        iterator = iterator->prev;
    } while (iterator);

    info("Key: %s not cached", key);
    return NOT_CACHED;
}

void cache(Cache* cache, const char* key, GLint value) {
    info("Caching (KEY: '%s', VALUE: %i)", key, value);

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

/* djb2 */
int hash(const char* key) {

    unsigned long hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % BUCKETS;
}

/* shit's leaking */
const char* getKey(const char* str, GLint num) {
    int len = strlen(str);
    int digits = floor(log10(abs(num))) + 1;
    size_t size = len + digits + 2;
    char* key = (char*) malloc(sizeof (char)*size);
    sprintf(key, "%i_%s", num, str);
    const char* rkey = (const char*) key;
    return rkey;
}

void printCache(Cache* cache, FILE* stream) {

    Bucket bit;
    Element* elit;

    for (int i = 0; i < BUCKETS; ++i) {
        bit = cache->buckets[i];
        elit = bit.last;

        fprintf(stream, "[%d] { ", i);

        if (!elit) {
            fprintf(stream, "}\n");
            continue;
        }

        do {
            fprintf(stream, "%s(%d) ", elit->key, elit->value);
            elit = elit->prev;
        } while (elit);

        fprintf(stream, "}\n");
    }
}