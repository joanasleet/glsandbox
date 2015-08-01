#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

#define ANISOTROPIC_LVL 16.0f

typedef struct {

    GLuint id;
    GLenum target;

    uint32 width;
    uint32 height;

} Texture;

uint8 *texData( const char *file, int *width, int *height, int *compr );

Texture *emptyTexture( int width, int height, GLenum target, uint8 genMipMaps);
Texture *fileTexture(const char *file, GLenum target, uint8 genMipMaps);
Texture *cubeTexture(const char **cubeFaces, uint8 allSame, uint8 genMipMaps);

void freeTexture(Texture *);

typedef struct {

    Texture **textures;
    uint32 texCount;

    uint8 _pad[4];

} Material;

Material *newMaterial();
void freeMaterial(Material *);

#endif

