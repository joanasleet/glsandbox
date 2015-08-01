#include "Util.h"
#include "Material.h"
#include "Engine.h"

#include "stb_image.h"

extern Engine *renderer;

Material *newMaterial() {

    Material *material = alloc( Material, 1 );
    return_guard(material, NULL);

    material->texCount = 0;
    material->textures = NULL;

    return material;
}

void freeMaterial(Material *mat) {

    for (uint32 i = 0; i < mat->texCount; ++i) {

        /* freeing textures disables reloads
         * as reloads need to free objects and
         * objects contain textures */
        //freeTexture(mat->textures[i]);
        
        mat->textures[i] = NULL;
    }

    free(mat->textures);
    free(mat);
}

uint8 *texData(const char *file, int *width, int *height, int *compr ) {

    /* check if tex data is cached */
    //lua_getfield( renderer->texDataCache, -1, file );
    //uint8 *data = lua_touserdata( renderer->texDataCache, -1 );
    //lua_pop( renderer->texDataCache, 1 );

    //if( data ) return data; 

    uint8 *data = stbi_load(file, width, height, compr, 4);
    log_info("<Texture %s>: %i x %i (%i) ", file, *width, *height, *compr);

    return_guard(data, NULL);

    /* texDataCache[file] = data */
    lua_pushlightuserdata( renderer->texDataCache, ( void* ) data );
    lua_setfield( renderer->texDataCache, -2, file );

    return data;
}

Texture *emptyTexture( int width, int height, GLenum target, uint8 genMipMaps ) {

    Texture *texture = alloc( Texture, 1 );
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = target;

    glBindTexture(target, texture->id);

    /* allocate memory */
    glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (genMipMaps) {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPIC_LVL);

    return texture;
}

Texture *fileTexture(const char *file, GLenum target, uint8 genMipMaps) {

    /* check if cached */
    lua_getfield( renderer->textureCache, -1, file );
    Texture *texture = lua_touserdata( renderer->textureCache, -1 );
    lua_pop( renderer->textureCache, 1 );

    if( texture ) return texture;

    texture = alloc( Texture, 1 );
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = target;

    /* textureCache[file] = texture */
    lua_pushlightuserdata( renderer->textureCache, ( void* ) texture );
    lua_setfield( renderer->textureCache, -2, file );

    glBindTexture(target, texture->id);

    /* allocate and send tex data */
    int width, height, compr;
    uint8 *data = texData( file, &width, &height, &compr );
    glTexImage2D(target, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    /* min/mag filter */
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* wrapping behaviour */
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (genMipMaps) {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    /* anisotropic filter level */
    glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPIC_LVL);

    return texture;
}

Texture *cubeTexture(const char **cubeFaces, uint8 allSame, uint8 genMipMaps) {

    /* check if cached */
    //lua_getfield( renderer->textureCache, -1, file );
    //Texture *texture = lua_touserdata( renderer->textureCache, -1 );
    //lua_pop( renderer->textureCache, 1 );

    //if( texture ) return texture;

    Texture *texture = alloc( Texture, 1 );
    return_guard(texture, NULL);

    glGenTextures(1, &texture->id);
    texture->target = GL_TEXTURE_CUBE_MAP;

    /* texIdCache[file] = texture->id */
    //lua_pushlightuserdata( renderer->textureCache, ( void* ) texture );
    //lua_setfield( renderer->textureCache, -2, file );

    glBindTexture(texture->target, texture->id);

    for (int face = 0; face < 6; face++) {

        int w, h, c;
        const char *file = cubeFaces[face];

        uint8 *faceData = texData( file, &w, &h, &c );

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    0, GL_SRGB_ALPHA, w, h, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, faceData);
    }

    /* min/mag filter */
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* wrapping behaviour */
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (genMipMaps) {
        glGenerateMipmap(texture->target);
        glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    return texture;
}

void freeTexture(Texture *tex) {
    if (!tex) return;

    glDeleteTextures(1, &(tex->id));
    free(tex);
}

