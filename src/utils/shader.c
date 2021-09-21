#include "shader.h"

void CheckCompileErrors(unsigned int object, const char* type)
{
    int success = 0;
    char infoLog[1024] = {""};
    if(type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Compile-time error: Type: %s \n %s \n -- --------------------------------------------------- -- ", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            printf("| ERROR::SHADER: Link-time error: Type: %s \n %s \n -- --------------------------------------------------- -- ", type, infoLog);
        }
    }
}

void UseShader(struct Shader *shader)
{
    glUseProgram(shader->ID);
}

struct Shader MakeShader(const char *vertexSource, const char *fragmentSource, const char *geometrySource)
{
    struct Shader shader;

    vertexSource = ReadFileToString(vertexSource);
    fragmentSource = ReadFileToString(fragmentSource);
    geometrySource = ReadFileToString(geometrySource);

    unsigned int sVertex, sFragment, gShader;

    // Vertex shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    CheckCompileErrors(sVertex, "VERTEX");

    // Fragment shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    CheckCompileErrors(sFragment, "FRAGMENT");

    // Geomtry shader if given
    if(geometrySource != NULL)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        CheckCompileErrors(gShader, "GEOMETRY");
    }

    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, sVertex);
    glAttachShader(shader.ID, sFragment);
    if(geometrySource != NULL)
        glAttachShader(shader.ID, gShader);
    glLinkProgram(shader.ID);
    CheckCompileErrors(shader.ID, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if(geometrySource != NULL)
        glDeleteShader(gShader);

    return shader;
}

void DeleteShader(struct Shader *shader)
{
    glDeleteProgram(shader->ID);
}

void SetShaderFloat(struct Shader *shader, const char *name, float value, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

void SetShaderInteger(struct Shader *shader, const char *name, int value, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

void SetShaderVector2f(struct Shader *shader, const char *name, float x, float y, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform2f(glGetUniformLocation(shader->ID, name), x, y);
}

void SetShaderVector2fv(struct Shader *shader, const char *name, const vec2s value, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform2f(glGetUniformLocation(shader->ID, name), value.x, value.y);
}

void SetShaderVector3f(struct Shader *shader, const char *name, float x, float y, float z, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform3f(glGetUniformLocation(shader->ID, name), x, y, z);
}

void SetShaderVector3fv(struct Shader *shader, const char *name, const vec3s value, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform3f(glGetUniformLocation(shader->ID, name), value.x, value.y, value.z);
}

void SetShaderVector4f(struct Shader *shader, const char *name, float x, float y, float z, float w, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform4f(glGetUniformLocation(shader->ID, name), x, y, z, w);
}

void SetShaderVector4fv(struct Shader *shader, const char *name, const vec4s value, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniform4f(glGetUniformLocation(shader->ID, name), value.x, value.y, value.z, value.w);
}

void SetShaderMatrix2(struct Shader *shader, const char *name, const mat2s matrix, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniformMatrix2fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (const float *)&matrix); 
}

void SetShaderMatrix3(struct Shader *shader, const char *name, const mat3s matrix, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniformMatrix3fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (const float *)&matrix); 
}

void SetShaderMatrix4(struct Shader *shader, const char *name, const mat4s matrix, bool useShader)
{
    if(useShader)
        UseShader(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (const float *)&matrix); 
}