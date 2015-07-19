#include "Util.h"
#include "Material.h"
#include "Engine.h"

#include "stb_image.h"

extern Engine *renderer;

Texture *nullTex = NULL;

Material *newMaterial() {

    Material *material = alloc( Material, 1 );
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

// NOTE: mipmaps might be wrong for height maps ?!

Texture *createTexture(const char *file, GLenum target, uint8 genMipMaps) {

    Texture *texture = alloc( Texture, 1 );
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = target;

    /* check if tex data is cached */
    lua_getfield( renderer->textureCache, -1, file );
    uint8 *data = lua_touserdata( renderer->textureCache, -1 );
    lua_pop( renderer->textureCache, 1 );

    if( !data ) {

        data = getData(file, &(texture->width), &(texture->height));

        /* textureCache[file] = data */
        lua_pushlightuserdata( renderer->textureCache, ( void* ) data );
        lua_setfield( renderer->textureCache, -2, file );
    }

    texture->data = data;

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
        log_warn("<Texture %s>: %i x %i (%i) ", file, *width, *height, texCompr);
    } else if (width && !height) {
        int h;
        data = stbi_load(file, width, &h, &texCompr, 4);
        log_warn("<Texture %s>: %i x %i (%i) ", file, *width, h, texCompr);
    } else if (!width && height) {
        int w;
        data = stbi_load(file, &w, height, &texCompr, 4);
        log_warn("<Texture %s>: %i x %i (%i) ", file, w, *height, texCompr);
    } else {
        int w, h;
        data = stbi_load(file, &w, &h, &texCompr, 4);
        log_warn("<Texture %s>: %i x %i (%i) ", file, w, h, texCompr);
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

    nullTex = alloc( Texture, 1 );
    return_guard(nullTex, NULL);

    glGenTextures(1, &nullTex->id);
    nullTex->target = GL_TEXTURE_2D;
    nullTex->data = NULL;
    nullTex->width = 0;
    nullTex->height = 0;

    return nullTex;
}

Texture *cubeTexture(const char **cubeFaces, uint8 allSame, uint8 genMipMaps) {

    Texture *texture = alloc( Texture, 1 );
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = GL_TEXTURE_CUBE_MAP;
    glBindTexture(texture->target, texture->id);
    texture->data = NULL;

    for (int face = 0; face < 6; face++) {

        int w, h;
        const char *file = cubeFaces[face];

        /* check if tex data is cached */
        lua_getfield( renderer->textureCache, -1, file );
        uint8 *faceData = lua_touserdata( renderer->textureCache, -1 );
        lua_pop( renderer->textureCache, 1 );

        if( !faceData ) {

            faceData = getData( file, &w, &h );

            /* textureCache[file] = data */
            lua_pushlightuserdata( renderer->textureCache, ( void* ) faceData );
            lua_setfield( renderer->textureCache, -2, file );
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
