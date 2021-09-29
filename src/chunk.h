#ifndef CHUNK_H
#define CHUNK_H

#include <glad/glad.h>

#include <cglm/struct.h>

#include "world.h"
#include "noise.h"
#include "structure.h"

#include "utils/shader.h"
#include "utils/vector.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16
#define TEXTURE_ATLAS_SIZE_IN_BLOCKS 16
// Without UV_OFFSET the texture may produce wierd artifact but I have tested it yet so it might not
#define UV_OFFSET 0.001f

defineVector(float);
defineVector(int);

struct Chunk
{
    vec3s position;

    unsigned int VAO, VBO, EBO, CBO, UVBO, indiceIndex;

    uint8_t voxels[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
    uint8_t lights[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];

    bool renderable, populated, modified, inVector;

    vectorvoxelmod modifications;
};

struct MeshData
{
    vectorfloat vertices, uvs, colors;
	vectorint indices;
};

struct VoxelMeshMod
{
    vec3s frontFace, backFace, leftFace, rightFace, topFace, bottomFace;
};

struct Voxel
{
    const char *name;

    float health, transparency;

    uint8_t frontFace, backFace, leftFace, rightFace, topFace, bottomFace, lightLevel;

    bool invisible, renderNeighborFaces, forceRenderFaces, cross, collider, renderNeighborsFrontST, renderNeighborsBackST, renderNeighborsLeftST, renderNeighborsRightST, renderNeighborsTopST, renderNeighborsBottomST;

    struct VoxelMeshMod voxelMeshMod;
};

struct Voxel voxels[19];

float chunkGlobalLightLevel;

uint8_t GetVoxel(struct Chunk *chunk, int x, int y, int z);

void MakeChunk(struct Chunk *chunk, int x, int y, int z);
void CreateVoxel(struct Chunk *chunk, struct MeshData *meshData, int x, int y, int z, uint8_t ID);
void CreateChunkBufferData(struct Chunk *chunk, struct MeshData *meshData);
void DeleteChunk(struct Chunk *chunk);
void ClearChunk(struct Chunk *chunk);
void RenderChunk(struct Chunk *chunk, struct Shader *shader);
void CreateVoxels(struct Chunk *chunk);
void CreateVertices(struct Chunk *chunk, bool shade);
void CalculateLight(struct Chunk *chunk);

bool IsVoxelInChunk(struct Chunk *chunk, int x, int y, int z);


#endif