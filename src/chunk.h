#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>

#include <cglm/struct.h>

#include "noise.h"
#include "utils/shader.h"
#include "utils/vector.h"

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

    unsigned int x, z, VBO, VAO, EBO, CBO, UVBO, indiceIndex;


    uint8_t voxels[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
};

struct Voxel
{
    const char *name;

    uint8_t frontFace, backFace, leftFace, rightFace, topFace, bottomFace;

    bool isInvisible, renderNeighborFaces;
};

struct Chunk MakeChunk(int x, int y, int z);

uint8_t GenerateVoxel(struct Chunk *chunk, int x, int y, int z);
uint8_t GetVoxel(struct Chunk *chunk, int x, int y, int z);

//void CreateChunkData(struct Chunk *chunk);
void CreateVoxel(struct Chunk *chunk, int x, int y, int z, uint8_t ID);
//void CreateChunk(struct Chunk *chunk);
void CreateChunkBufferData(struct Chunk *chunk);
void DeleteChunk(struct Chunk *chunk);
void RenderChunk(struct Chunk *chunk, struct Shader *shader);
void CreateVoxels(struct Chunk *chunk);
void CreateVertices(struct Chunk *chunk);


#endif