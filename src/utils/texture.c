#include "texture.h"

#include <stb_image.h>

struct Texture2D MakeTexture(const char *file, struct Texture2D *texture, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);

    GenerateTexture(texture, width, height, data);
    stbi_image_free(data);
    return *texture;
}

struct Texture2D CreateTexture()
{
    struct Texture2D texture;
    texture.width = 0;
    texture.height = 0;
    texture.internalFormat = GL_RGB;
    texture.imageFormat = GL_RGB;
    texture.wrapS = GL_REPEAT;
    texture.wrapT = GL_REPEAT;
    texture.filterMin = GL_LINEAR;
    texture. filterMax = GL_LINEAR;

    glGenTextures(1, &texture.ID);

    return texture;
}

struct Texture2D CreateTextureData(unsigned int internalFormat, unsigned int imageFormat, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMax)
{
    struct Texture2D texture;
    texture.width = 0;
    texture.height = 0;
    texture.internalFormat = internalFormat;
    texture.imageFormat = imageFormat;
    texture.wrapS = wrapS;
    texture.wrapT = wrapT;
    texture.filterMin = filterMin;
    texture. filterMax = filterMax;

    glGenTextures(1, &texture.ID);

    return texture;
}

void GenerateTexture(struct Texture2D *texture, unsigned int width, unsigned int height, unsigned char *data)
{
    texture->width = width;
    texture->height = height;

    glBindTexture(GL_TEXTURE_2D, texture->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->internalFormat, width, height, 0, texture->imageFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);

}

void DeleteTexture(struct Texture2D *texture)
{
    glDeleteTextures(1, &texture->ID);
}

void BindTexture(struct Texture2D *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->ID);
}