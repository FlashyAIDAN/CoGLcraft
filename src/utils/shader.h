#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

struct Shader
{
    unsigned int ID;
};

struct Shader MakeShader(const char *vertexSource, const char *fragmentSource, const char *geometrySource);

void UseShader(struct Shader *shader);
void DeleteShader(struct Shader *shader);
void SetShaderFloat(struct Shader *shader, const char *name, float value, bool useShader);
void SetShaderInteger(struct Shader *shader, const char *name, int value, bool useShader);
void SetShaderVector2f(struct Shader *shader, const char *name, float x, float y, bool useShader);
void SetShaderVector2fv(struct Shader *shader, const char *name, const vec2s value, bool useShader);
void SetShaderVector3f(struct Shader *shader, const char *name, float x, float y, float z, bool useShader);
void SetShaderVector3fv(struct Shader *shader, const char *name, const vec3s value, bool useShader);
void SetShaderVector4f(struct Shader *shader, const char *name, float x, float y, float z, float w, bool useShader);
void SetShaderVector4fv(struct Shader *shader, const char *name, const vec4s value, bool useShader);
void SetShaderMatrix2(struct Shader *shader, const char *name, const mat2s matrix, bool useShader);
void SetShaderMatrix3(struct Shader *shader, const char *name, const mat3s matrix, bool useShader);
void SetShaderMatrix4(struct Shader *shader, const char *name, const mat4s matrix, bool useShader);

#endif