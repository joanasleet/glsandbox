#include "Texture.h"

GLenum freeTexSlot = GL_TEXTURE0;
int maxTexSlots;

Texture* newTexture(const char* file, GLenum target, bool genMipMaps) {
    Texture* tex = (Texture*) malloc(sizeof (Texture));
    if (!tex) {
        ERR("Failed to allocate texture <%s>", file);
        return NULL;
    }

    glGenTextures(1, &tex->id);
    tex->target = target;
    glBindTexture(tex->target, tex->id);

    glActiveTexture(freeTexSlot++);

    tex->data = texData(file, &tex->width, &tex->height);

    glTexImage2D(tex->target, 0, GL_RGBA, tex->width, tex->height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);

    glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (genMipMaps) {
        glGenerateMipmap(tex->target);
        glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    return tex;
}

unsigned char* texData(const char* file, int* width, int* height) {

    unsigned char* data;
    int texCompr;

    if (width && height) {
        data = stbi_load(file, width, height, &texCompr, 4);
        INFO("Texture <%s>: %i x %i (%i) ", file, *width, *height, texCompr);
    } else if (width && !height) {
        int h;
        data = stbi_load(file, width, &h, &texCompr, 4);
        INFO("Texture <%s>: %i x %i (%i) ", file, *width, h, texCompr);
    } else if (!width && height) {
        int w;
        data = stbi_load(file, &w, height, &texCompr, 4);
        INFO("Texture <%s>: %i x %i (%i) ", file, w, *height, texCompr);
    } else {
        int w, h;
        data = stbi_load(file, &w, &h, &texCompr, 4);
        INFO("Texture <%s>: %i x %i (%i) ", file, w, h, texCompr);
    }

    if (!data) {
        ERR("Failed to load texture <%s>", file);
        return NULL;
    }

    return data;
}

void bind(Texture* tex) {
    glActiveTexture(tex->slot);
    glBindTexture(tex->target, tex->id);
}

Texture* cubeMap(const char* cubeFaces[], bool allSame, bool genMipMaps) {
    
    Texture* tex = (Texture*) malloc(sizeof (Texture));
    if (!tex) {
        ERR("Failed to allocate cube map texture");
        return NULL;
    }

    glGenTextures(1, &tex->id);
    tex->target = GL_TEXTURE_CUBE_MAP;
    glBindTexture(tex->target, tex->id);

    glActiveTexture(freeTexSlot++);

    int w, h;
    unsigned char* faceTexBuffer;

    if (allSame) {
        faceTexBuffer = texData(cubeFaces[0], &w, &h);
    }

    for (int face = 0; face < 6; face++) {
        if (!allSame) {
            faceTexBuffer = texData(cubeFaces[face], &w, &h);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, faceTexBuffer);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    if (genMipMaps) {
        glGenerateMipmap(tex->target);
        glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    return tex;
}