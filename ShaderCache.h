#ifndef SHADERCACHE_H
#define	SHADERCACHE_H

#include "common.h"

#define BUCKETS 10
#define NOT_CACHED 0

typedef struct Element {

    const char* key;
    GLint value;

    Element* next;
    Element* prev;
} Element;

Element* newElement(const char* key, GLint value);

typedef struct Bucket {
    Element* last;
} Bucket;

typedef struct Hash {
    Bucket buckets[BUCKETS];

} Hash, ShaderCache, UniformCache;

Hash* newCache();

GLint get(Hash* cache, const char* key);
void cache(Hash* cache, const char* key, GLint value);

int hash(const char* key);

const char* getKey(const char* str, GLint num);

void printCache(Hash* cache);

#endif	/* SHADERCACHE_H */

