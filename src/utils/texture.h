#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <stdbool.h>

struct Texture2D
{
    unsigned int ID, width, height, internalFormat, imageFormat, wrapS, wrapT, filterMin, filterMax;
};

struct Texture2D MakeTexture(const char *file, struct Texture2D *texture, bool flip);
struct Texture2D CreateTexture();
struct Texture2D CreateTextureData(unsigned int internalFormat, unsigned int imageFormat, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMax);

void GenerateTexture(struct Texture2D *texture, unsigned int width, unsigned int height, unsigned char *data);
void DeleteTexture(struct Texture2D *texture);
void BindTexture(struct Texture2D *texture);

#endif