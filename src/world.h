#ifndef WORLD_H
#define WORLD_H

#include "utils/texture.h"
#include "utils/shader.h"
#include "utils/vector.h"

#include "chunk.h"

#define NUMBER_OF_CHUNKS_X 100
#define NUMBER_OF_CHUNKS_Z 100

typedef struct // Testing purposes
{
	int x, y;
} ivec2s;

defineVector(ivec2s);

vectorivec2s activeChunks;
vectorivec2s updateMesh;

void WorldStart(/*vectorivec2s activeChunks, vectorivec2s meshToCreate*/);
void WorldRender(struct Texture2D *texture, struct Shader *shader);
void WorldDelete();
void UpdateViewDistance(ivec2s currentChunk/*, vectorivec2s activeChunks, vectorivec2s meshToCreate*/);

uint8_t WorldGetVoxel(int x, int y, int z);

ivec2s GetCurrentChunkCoordinates(float x, float z);

bool IsVoxelInWorld(int x, int y, int z);
bool IsChunkInWorld(int x, int z);

#endif