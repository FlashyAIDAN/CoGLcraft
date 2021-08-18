#include "shader.h"

#include <stdio.h>

// TODO(Aidan): Can only read files with the "LF" line ending
// ReadFile funtion I got from https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
char* ReadFile(const char *filename)
{
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");
    
    if (handler)
    {
        // Seek the last byte of the file
        fseek(handler, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(handler);
        // go back to the start of the file
        rewind(handler);
        
        // Allocate a string that can hold it all
        buffer = (char*) malloc(sizeof(char) * (string_size + 1) );
        
        // Read it all in one operation
        read_size = fread(buffer, sizeof(char), string_size, handler);
        
        // fread doesn't set it so put a \0 in the last position
        // and buffer is now officially a string
        buffer[string_size] = '\0';
        
        if (string_size != read_size)
        {
            // Something went wrong, throw away the memory and set
            // the buffer to NULL
            free(buffer);
            buffer = NULL;
        }
        
        // Always remember to close the file.
        fclose(handler);
    }
    
    return buffer;
}

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

    vertexSource = ReadFile(vertexSource);
    fragmentSource = ReadFile(fragmentSource);
    geometrySource = ReadFile(geometrySource);

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