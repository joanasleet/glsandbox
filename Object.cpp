#include "Object.h"

Object* newObj(GLenum target) {

    Object* obj = (Object*) malloc(sizeof (Object));
    obj->target = target;

    switch (target) {

        case GL_VERTEX_ARRAY:
            glGenVertexArrays(1, &obj->id);
            obj->bufferId = FIELD_UNUSED;
            break;

        case GL_ARRAY_BUFFER:
        case GL_ELEMENT_ARRAY_BUFFER:
            glGenBuffers(1, &obj->id);
            obj->bufferId = obj->id;
            break;

        case GL_TEXTURE_1D:
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
            glGenTextures(1, &obj->id);
            obj->bufferId = FIELD_UNUSED;
            break;

        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
            glGenSamplers(1, &obj->id);
            obj->bufferId = FIELD_UNUSED;
            break;

        default:
            ERR("Unknown bind target (%d).", target);
            break;
    }

    return obj;
}

Object* texture(const char* file, GLenum target, GLenum slot, bool loadToGPU) {

    Object* texObj = newObj(target);
    glBindTexture(texObj->target, texObj->id);

    glActiveTexture(slot);

    int imgWidth, imgHeight, imgCompression;
    unsigned char* texData = texBuffer(file, &imgWidth, &imgHeight, &imgCompression);
    texObj->texBuffer = texData;
    texObj->texWidth = imgWidth;
    texObj->texHeight = imgHeight;

    if (!loadToGPU)
        return texObj;

    glTexImage2D(texObj->target, 0, GL_RGBA, texObj->texWidth, texObj->texHeight,
            0, GL_RGBA, GL_UNSIGNED_BYTE, texObj->texBuffer);

    glTexParameteri(texObj->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texObj->target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(texObj->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(texObj->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texObj;
}

unsigned char* texBuffer(const char* file, int* width, int* height, int* compression, int channels) {

    unsigned char* tex_data = stbi_load(file, width, height, compression, channels);

    if (!tex_data) {
        ERR("Failed to load texture from file <%s>", file);
    }
    INFO("Texture attribute: %i x %i (%i) <%s>", *width, *height, *compression, file);

    return tex_data;
}

void bind(Object* obj) {

}
