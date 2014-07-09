#ifndef OBJECT_H
#define	OBJECT_H

#include "common.h"

#define FIELD_UNUSED 0

typedef struct Object {
    GLuint id;
    GLenum target;

    GLuint bufferId;

    unsigned char* texBuffer;
    int texWidth;
    int texHeight;
} Object;

Object* newObj(GLenum target);

Object* texture(const char* file, GLenum target, GLenum slot = GL_TEXTURE0,
        bool loadToGPU = true);

unsigned char* texBuffer(const char* file, int* width, int* height,
        int* compression, int channels = 4);

void bind(Object* obj);

void execByTarget(Object* obj, void(* func)());

#endif	/* OBJECT_H */

