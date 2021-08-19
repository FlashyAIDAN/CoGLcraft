#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "utils/shader.h"
#include "utils/vectorf.h"
#include "utils/vectori.h"

#include <stdint.h>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16
#define TEXTURE_ATLAS_SIZE_IN_BLOCKS 16
// Without UV_OFFSET the texture may produce wierd artifact but I have tested it yet so it might not
#define UV_OFFSET 0.001f

struct Chunk
{
    vectorf vertices, uvs, colors;
	vectori indices;

    vec3s position;

    unsigned int VBO, VAO, EBO, CBO, UVBO, indiceIndex;
};

struct Block
{
    const char *name;

    uint8_t ID, frontFace, backFace, leftFace, rightFace, topFace, bottomFace;

    bool isInvisible;
};

struct Chunk MakeChunk(int x, int y, int z);

void CreateChunkMesh(struct Chunk *chunk);
void CreateVoxel(struct Chunk *chunk, int x, int y, int z, uint8_t ID);
void CreateChunk(struct Chunk *chunk);
void CreateChunkBufferData(struct Chunk *chunk);
void DeleteChunk(struct Chunk *chunk);
void RenderChunk(struct Chunk *chunk, struct Shader *shader);

#endif