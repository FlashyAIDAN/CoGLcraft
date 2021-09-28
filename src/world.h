#ifndef WORLD_H
#define WORLD_H

#include "utils/texture.h"
#include "utils/shader.h"
#include "utils/vector.h"

#include "vendor/ivec2s.h"
#include "vendor/ivec3s.h"

#include <stdlib.h>

#define NUMBER_OF_CHUNKS_X 100
#define NUMBER_OF_CHUNKS_Z 100

defineVector(ivec2s);
defineVector(ivec3s);

struct BiomeBlock
{
	uint8_t ID;
	 int minHeight, maxHeight;
};

enum StructureType
{
	TREE,
	CACTUS,
	PLANT
};

struct Structure
{
	enum StructureType type;
	int minHeight, maxHeight, height;
	uint8_t ID;
	float offset, scale, threshold, subOffset, subScale, subThreshold;
};

struct Lode
{
	const char *name;
	uint8_t ID, replaceVoxel; // Maybe make "replaceVoxel" a array/vector
	int minHeight, maxHeight;
	float offset, scale, threshold;
};

struct Biome
{
	const char *name;
	int terrainHeight;
	float offset, scale, terrainOffset, terrainScale;
	//vectorstructure structures;
	struct BiomeBlock *blocks;
	struct Structure *structures;
	struct Lode *lodes;
	int sizeofBlocks, sizeofStructures, sizeofLodes;
};

typedef struct VoxelMod
{
    ivec3s position;
    uint8_t ID;
} voxelmod;
defineVector(voxelmod)
defineVector(vectorvoxelmod);
#include "chunk.h"

struct Dimension
{
	const char *name;
	struct Chunk *chunks[NUMBER_OF_CHUNKS_X][NUMBER_OF_CHUNKS_Z];
	struct Biome *biomes;
	int sizeofBiomes;
};

int viewDistance;

vectorivec2s activeChunks;
vectorivec2s updateMesh;
vectorivec2s modifyMesh;
vectorivec2s makeMesh;

vectorvectorvoxelmod modifications;

void WorldStart(struct Shader *shader, ivec2s currentChunk);
void WorldRender(struct Texture2D *texture, struct Shader *shader);
void WorldDelete();
void UpdateViewDistance(ivec2s currentChunk);
void EditVoxel(struct Chunk *chunk, ivec3s position, uint8_t ID);

struct Chunk *GetChunk(ivec2s position);

uint8_t WorldGetVoxel(int x, int y, int z);
uint8_t GenerateVoxel(vec3s position, int x, int y, int z);

ivec2s GetCurrentChunkCoordinates(vec2s position);

ivec3s GetBlockLookedAt(vec3s position, vec3s front, float reach, float increment, bool normalized);

bool IsVoxelInWorld(int x, int y, int z);
bool IsChunkInWorld(ivec2s position);

#endif