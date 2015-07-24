#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

#define ANISOTROPIC_LVL 16.0f

typedef struct {

    GLuint id;
    GLenum target;

    int32 width;
    int32 height;

} Texture;

Texture *newTexture(const char *file, GLenum target, uint8 genMipMaps);
uint8 *getData(const char *file, int *width, int *height);
void freeTexture(Texture *);

Texture *cubeTexture(const char **cubeFaces, uint8 allSame, uint8 genMipMaps);
Texture *nullTexture();

typedef struct {

    uint32 texCount;
    Texture **textures;

} Material;

Material *newMaterial();
void freeMaterial(Material *);

#endif

