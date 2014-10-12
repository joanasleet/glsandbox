#ifndef TEXTURE_H
#define	TEXTURE_H

#include "common.h"

#define ANISOTROPIC_LVL 4.0f

//extern GLenum freeTexSlot; // managed by context
extern int32 maxTexSlots;

typedef struct {
    GLuint id;
    GLenum target;

    int32 width;
    int32 height;

    uint8* data;

} Texture;

Texture* newTexture(const char* file, GLenum target = GL_TEXTURE_2D, bool genMipMaps = true);
uint8* getData(const char* file, int* width = NULL, int* height = NULL);
void freeTexture(Texture*);

Texture* cubeTexture(const char* *cubeFaces, bool allSame = false, bool genMipMaps = false);
Texture* nullTexture();

typedef struct {
    Texture* diffuseMap;
    Texture* ambientMap;
    Texture* specularMap;
} Material;

Material* newMaterial();
void freeMaterial(Material*);

#endif	/* TEXTURE_H */

