#ifndef TEXTURE_H
#define	TEXTURE_H

#include "common.h"

#define ANISOTROPIC_LVL 4.0f

extern GLenum freeTexSlot; // managed by context
extern int maxTexSlots;

typedef struct Texture {
    GLuint id;
    GLenum target;

    /*
     * gpu texture slot */
    GLenum slot;

    /* 
     * light attributes */
    GLfloat kAmb, kDiff, kSpec;

    /*
     * image attributes */
    int width, height;
    unsigned char* data, mipMaps;

} Texture, Tex;

Texture* newTexture(const char* file, GLenum target = GL_TEXTURE_2D, bool genMipMaps = true);
unsigned char* texData(const char* file, int* width = NULL, int* height = NULL);
void bind(Texture* tex);

Texture* cubeMap(const char* cubeFaces[], bool allSame = false, bool genMipMaps = false);

Texture* nullTexture();

#endif	/* TEXTURE_H */

