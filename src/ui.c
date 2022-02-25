#include "ui.h"

struct UIrenderer MakeUIrenderer(struct Shader shader, unsigned int width, unsigned int height)
{
    struct UIrenderer uiRenderer;
    uiRenderer.shader = shader;
    uiRenderer.VAO = 0;
    SetShaderInteger(&uiRenderer.shader, "sprite", 0, true);
	SetShaderMatrix4(&uiRenderer.shader, "projection", glms_ortho(-width, width, -height, height, -1.0f, 1.0f), false);
    
    float uiVertices[16] =
	{
        // pos      // tex     // colors           // texture coords
        -0.5f, -0.5f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 1.0f, 0.0f, // bottom right
        0.5f, 0.5f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f  // top left 
    };

	unsigned int uiIndices[6] =
	{
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int uiVBO, uiEBO = 0;

    glGenVertexArrays(1, &uiRenderer.VAO);
    glGenBuffers(1, &uiVBO);
    glGenBuffers(1, &uiEBO);
    glBindVertexArray(uiRenderer.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uiVertices), uiVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uiIndices), uiIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return uiRenderer;
}

void RenderSpriteUI(struct UIrenderer *uiRenderer, struct Texture2D *texture, vec2s position, float rotation, vec2s size, vec3s color)
{
    UseShader(&uiRenderer->shader);
	mat4s uiModelBlock = (mat4s){ 1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f };
		
	uiModelBlock = glms_translate(uiModelBlock, (vec3s){position.x, position.y, 0.0f});
	uiModelBlock = glms_rotate(uiModelBlock, glm_rad(rotation), (vec3s){0.0f, 0.0f, 1.0f});
    uiModelBlock = glms_scale(uiModelBlock, (vec3s){size.x, size.y, 1.0f});

	SetShaderMatrix4(&uiRenderer->shader, "model", uiModelBlock, false);

    // render textured quad
   	SetShaderVector3fv(&uiRenderer->shader, "spriteColor", color, false);

    glActiveTexture(GL_TEXTURE0);
    BindTexture(texture);

    glBindVertexArray(uiRenderer->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void DeleteUIrenderer(struct UIrenderer *uiRenderer)
{
    DeleteShader(&uiRenderer->shader);
    glDeleteVertexArrays(1, &uiRenderer->VAO);
}