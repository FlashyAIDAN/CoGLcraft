#ifndef WORLD_H
#define WORLD_H

#include "utils/texture.h"
#include "utils/shader.h"
#include "utils/vector.h"

#include "vendor/ivec2s.h"
#include "vendor/ivec3s.h"

#define NUMBER_OF_CHUNKS_X 100
#define NUMBER_OF_CHUNKS_Z 100

defineVector(ivec2s);

typedef struct VoxelMod
{
    ivec3s position;
    uint8_t ID;
} voxelmod;

defineVector(voxelmod)
defineVector(vectorvoxelmod);
#include "chunk.h"

struct Lode
{
	const char *name;
	uint8_t ID, replaceVoxel; // Maybe make "replaceVoxel" a array/vector
	unsigned int minHeight, maxHeight;
	float scale, offset, threshold;
};

int viewDistance;

vectorivec2s activeChunks;
vectorivec2s updateMesh;
vectorivec2s modifyMesh;

vectorvectorvoxelmod modifications;

void WorldStart();
void WorldRender(struct Texture2D *texture, struct Shader *shader);
void WorldDelete();
void UpdateViewDistance(ivec2s currentChunk);
void EditVoxel(struct Chunk *chunk, ivec3s position, uint8_t ID);

struct Chunk *GetChunk(int x, int z);

uint8_t WorldGetVoxel(int x, int y, int z);
uint8_t GenerateVoxel(vec3s position, int x, int y, int z);

ivec2s GetCurrentChunkCoordinates(float x, float z);

ivec3s GetBlockLookedAt(vec3s position, vec3s front, float reach, float increment, bool normalized);

bool IsVoxelInWorld(int x, int y, int z);
bool IsChunkInWorld(int x, int z);

#endif