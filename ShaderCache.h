#ifndef SHADERCACHE_H
#define	SHADERCACHE_H

#include "common.h"

#define BUCKETS 10

typedef struct Element {
    int index;

    char* key;
    GLint value;

    Element* next;
    Element* prev;
} Element;

Element* newElement(int indx, char* key, GLint value);

typedef struct Bucket {
    Element* last;

    Bucket* next;
    Bucket* prev;
} Bucket;

void add(Element* e, Bucket* b);
void remove(Element* e, Bucket* b);

typedef struct Hash {
    Bucket buckets[BUCKETS];

} Hash, ShaderCache, UniformCache;

Hash* newCache();

GLuint get(Hash* cache, const char* key);
void cache(Hash* cache, const char* key, GLint value);

int hash(char* key);

char* getKey(char* str, GLint num);


#endif	/* SHADERCACHE_H */

