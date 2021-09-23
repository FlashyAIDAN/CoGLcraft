#ifndef UI_H
#define UI_H

// TODO(Aidan): I cant get this to work right now so i'll come back in the future to redo it

#include "utils/shader.h"
#include "utils/texture.h"
#include "utils/emath.h"
#include "cglm/struct.h"

struct UIrenderer
{
    struct Shader shader;
    unsigned int VAO;
};

struct UIrenderer MakeUIrenderer(struct Shader shader, unsigned int width, unsigned int height);
void RenderSpriteUI(struct UIrenderer *uiRenderer, struct Texture2D *texture, vec2s position, float rotation, vec2s size, vec3s color);
void DeleteUIrenderer(struct UIrenderer *uiRenderer);

#endif