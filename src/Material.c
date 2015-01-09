#include "Material.h"
#include "Debugger.h"
#include "Deallocator.h"

#include "stb_image.h"

Texture *nullTex = NULL;

Material *newMaterial() {

    Material *material = (Material *) malloc(sizeof (Material));
    return_guard(material, NULL);

    material->texCount = 0;
    material->textures = NULL;

    return material;
}

void freeMaterial(Material *mat) {

    for (uint32 i = 0; i < mat->texCount; ++i) {
        freeTexture(mat->textures[i]);
        mat->textures[i] = NULL;
    }

    free(mat->textures);
    free(mat);
}

Texture *createTexture(const char *file, GLenum target, uint8 genMipMaps) {
    Texture *texture = (Texture *) malloc(sizeof (Texture));
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = target;
    texture->data = getData(file, &(texture->width), &(texture->height));

    glBindTexture(target, texture->id);

    uint32 width = texture->width;
    uint32 height = texture->height;

    glTexImage2D(target, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texture->data);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (genMipMaps) {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPIC_LVL);

    return texture;
}

uint8 *getData(const char *file, int *width, int *height) {

    uint8 *data;
    int texCompr;

    if (width && height) {
        data = stbi_load(file, width, height, &texCompr, 4);
        log_info("<Texture %s>: %i x %i (%i) ", file, *width, *height, texCompr);
    } else if (width && !height) {
        int h;
        data = stbi_load(file, width, &h, &texCompr, 4);
        log_info("<Texture %s>: %i x %i (%i) ", file, *width, h, texCompr);
    } else if (!width && height) {
        int w;
        data = stbi_load(file, &w, height, &texCompr, 4);
        log_info("<Texture %s>: %i x %i (%i) ", file, w, *height, texCompr);
    } else {
        int w, h;
        data = stbi_load(file, &w, &h, &texCompr, 4);
        log_info("<Texture %s>: %i x %i (%i) ", file, w, h, texCompr);
    }

    return_guard(data, NULL);
    return data;
}

void freeTexture(Texture *tex) {
    if (!tex) return;

    glDeleteTextures(1, &(tex->id));
    free(tex->data);
    free(tex);
}

Texture *nullTexture() {

    if (nullTex) return nullTex;

    nullTex = (Texture *) malloc(sizeof (Texture));
    return_guard(nullTex, NULL);

    glGenTextures(1, &nullTex->id);
    nullTex->target = GL_TEXTURE_2D;
    nullTex->data = NULL;
    nullTex->width = 0;
    nullTex->height = 0;

    storeBuffer(nullTex);
    return nullTex;
}

Texture *cubeTexture(const char **cubeFaces, uint8 allSame, uint8 genMipMaps) {

    Texture *texture = (Texture *) malloc(sizeof (Texture));
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = GL_TEXTURE_CUBE_MAP;
    glBindTexture(texture->target, texture->id);
    texture->data = NULL;

    int w, h;
    uint8 *faceData;

    if (allSame) {
        faceData = getData(cubeFaces[0], &w, &h);
        texture->data = faceData;
    }

    for (int face = 0; face < 6; face++) {
        if (!allSame) {
            faceData = getData(cubeFaces[face], &w, &h);

            /* * * * * * * * * * * * * * * * * * *
             * cant find a good model to unify   *
             * cubemaps with textues, so buffers *
             * like these are still freed        *
             * * * * * * * * * * * * * * * * * * */
            storeBuffer(faceData);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_SRGB_ALPHA, w, h, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, faceData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (genMipMaps) {
        glGenerateMipmap(texture->target);
        glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    return texture;
}