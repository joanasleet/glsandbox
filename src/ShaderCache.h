#ifndef SHADERCACHE_H
#define SHADERCACHE_H

#include "common.h"

#define BUCKETS 10
#define NOT_CACHED 0

typedef struct Element {
    const char *key;
    GLint value;

    struct Element *next;
    struct Element *prev;
} Element;

Element *newElement(const char *key, GLint value);

typedef struct {
    Element *last;
} Bucket;

typedef struct {
    Bucket buckets[BUCKETS];
} Cache;

Cache *newCache();
void freeCache(Cache *hash);
void clearCache(Cache *hash);

GLint get(Cache *cache, const char *key);
void cache(Cache *cache, const char *key, GLint value);

int hash(const char *key);

const char *getKey(const char *str, GLint num);

void printCache(Cache *cache, FILE *stream);

#endif  /* SHADERCACHE_H */


