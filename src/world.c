#include "world.h"

defineFuntionsVector(ivec2s, ((ivec2s){0.0f, 0.0f}));
defineFuntionsVector(vectorvoxelmod, ((vectorvoxelmod){0}));

struct Lode lodes[3] =
{
	{"Iron Ore", 12, 3, 256, 30, 0.0f, 0.25f, 0.7f},
	{"Coal Ore", 13, 3, 256, 0, 500.0f, 0.125f, 0.75f},
	{"Basic Cave", 0, 3, 100, 60, 1250.0f, 0.075f, 0.5f}
};

struct Biome biomes[] =
{
	{"Forest", 100, 0.0f, 0.125f, 0.0f, 0.25f, NULL, NULL, NULL, 3, 2, 3},
	{"Desert", 100, 500.0f, 0.125f, 0.0f, 0.25f, NULL, NULL, NULL, 1, 1, 1}
	//{"Mountain", 125, 500.0f, 0.125f, 0.0f, 0.25f, NULL, NULL, NULL}
};

struct Dimension dimensions[] =
{
	{"Overworld", {0}, NULL, 2}
};

int wnormals[6][3] = 
{
    { 0,  0,  1},
    { 0,  0, -1},
    {-1,  0,  0},
    { 1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0}
};

void WorldStart(struct Shader *shader)
{
	// BIOME 1
	biomes[0].blocks = calloc(3, sizeof(struct BiomeBlock));
	biomes[0].structures = calloc(2, sizeof(struct Structure));
	biomes[0].lodes = calloc(3, sizeof(struct Lode));
	biomes[0].blocks[0] = (struct BiomeBlock){2, 0, 0};
	biomes[0].blocks[1] = (struct BiomeBlock){1, 5, 1};
	biomes[0].blocks[2] = (struct BiomeBlock){3, 256, 6};
	biomes[0].structures[0] = (struct Structure){PLANT, 0, 0, 0, 18, 1000.0f, 2.0f, 0.3f, 0.0f, 15.0f, 0.5f}; // Make sure this is first or else they will replace the base trunk of trees ifd they overlap
	biomes[0].structures[1] = (struct Structure){TREE, 4, 7, 0, 0, 0.0f, 1.3f, 0.6f, 0.0f, 15.0f, 0.5f};
	biomes[0].lodes[0] = lodes[0];
	biomes[0].lodes[1] = lodes[1];
	biomes[0].lodes[2] = lodes[2];

	// BIOME 2
	biomes[1].blocks = calloc(1, sizeof(struct BiomeBlock));
	biomes[1].structures = calloc(1, sizeof(struct Structure));
	biomes[1].lodes = calloc(1, sizeof(struct Lode));
	biomes[1].blocks[0] = (struct BiomeBlock){16, 256, 0};
	biomes[1].structures[0] = (struct Structure){CACTUS, 5, 10, 0, 0, 750.0f, 1.3f, 0.6f, 0.0f, 15.0f, 0.5f};
	biomes[1].lodes[0] = lodes[1];

	// // BIOME 3
	// biomes[2].blocks = calloc(1, sizeof(struct BiomeBlock));
	// biomes[2].structures = calloc(0, sizeof(struct Structure));
	// biomes[2].lodes = calloc(2, sizeof(struct Lode));
	// biomes[2].blocks[0] = (struct BiomeBlock){3, 256, 0};
	// biomes[2].lodes[0] = lodes[0];
	// biomes[2].lodes[1] = lodes[1];
	// biomes[2].sizeofBlocks = 1;
	// biomes[2].sizeofStructures = 0;
	// biomes[2].sizeofLodes = 2;

	dimensions[0].biomes = calloc(2, sizeof(struct Biome));
	dimensions[0].biomes[0] = biomes[0];
	dimensions[0].biomes[1] = biomes[1];

	if(viewDistance == 0)
		viewDistance = 4;

	SetShaderFloat(shader, "fogNear", viewDistance * CHUNK_SIZE_X - 25.0f, true);
	SetShaderFloat(shader, "fogFar", viewDistance * CHUNK_SIZE_X, false);
	
	VectorInitivec2s(&activeChunks);
	VectorInitivec2s(&updateMesh);
	VectorInitivec2s(&modifyMesh);

	VectorInitvectorvoxelmod(&modifications);

	for (int x = (NUMBER_OF_CHUNKS_X / 2) - viewDistance; x < (NUMBER_OF_CHUNKS_X / 2) + viewDistance; x++)
	{
		for (int z = (NUMBER_OF_CHUNKS_Z / 2) - viewDistance; z < (NUMBER_OF_CHUNKS_Z / 2) + viewDistance; z++)
		{
			dimensions[0].chunks[x][z] = malloc(sizeof(struct Chunk));
			MakeChunk(dimensions[0].chunks[x][z], x, 0, z);
			VectorPushBackivec2s(&updateMesh, (ivec2s){x, z});
			VectorPushBackivec2s(&activeChunks, (ivec2s){x, z});
		}
	}
}

void WorldRender(struct Texture2D *texture, struct Shader *shader)
{
	
	if(VectorTotalivec2s(&updateMesh) > 0)
	{
		CreateVoxels(dimensions[0].chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y]);
		// int count = 0;
		while (VectorTotalvectorvoxelmod(&modifications) > 0)
		{
			vectorvoxelmod queue = VectorGetvectorvoxelmod(&modifications, 0);
			VectorDeletevectorvoxelmod(&modifications, 0);
			while (VectorTotalvoxelmod(&queue) > 0)
			{
				struct VoxelMod voxel = VectorGetvoxelmod(&queue, 0);
				VectorDeletevoxelmod(&queue, 0);

				if(IsChunkInWorld((int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)))
				{
					if (dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)] == 0)
					{
						dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)] = malloc(sizeof(struct Chunk));
						MakeChunk(dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)], (int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), 0, (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z));
						VectorPushBackivec2s(&updateMesh, (ivec2s){(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)});
					}
					else if(!dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)]->modified && dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)]->renderable)
					{
						VectorPushBackivec2s(&modifyMesh, (ivec2s){(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)});
						dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)]->modified = true;
					}
					VectorPushBackvoxelmod(&dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)]->modifications, voxel);
				}
			}
			// count++;
			// if (count > 200) // Splits work up each frame for every 200 voxels
				// return;
		}
		CreateVertices(dimensions[0].chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y]);
	 	CreateChunkBufferData(dimensions[0].chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y]);
		VectorDeleteivec2s(&updateMesh, 0);
	}
	while(VectorTotalivec2s(&modifyMesh) > 0)
	{
		ClearChunk(dimensions[0].chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]);
		CreateVertices(dimensions[0].chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]);
	 	CreateChunkBufferData(dimensions[0].chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]);
		dimensions[0].chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]->modified = false;
		VectorDeleteivec2s(&modifyMesh, 0);
	}
	
	BindTexture(texture);
	for(int i = 0; i < VectorTotalivec2s(&activeChunks); i++)
		if(dimensions[0].chunks[VectorGetivec2s(&activeChunks, i).x][VectorGetivec2s(&activeChunks, i).y]->renderable)
			RenderChunk(dimensions[0].chunks[VectorGetivec2s(&activeChunks, i).x][VectorGetivec2s(&activeChunks, i).y], shader);
}

void WorldDelete()
{
	VectorFreeivec2s(&activeChunks);
	VectorFreeivec2s(&updateMesh);
	VectorFreeivec2s(&modifyMesh);
    for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
	{
		for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
		{
			DeleteChunk(dimensions[0].chunks[x][z]);
		}
	}
}

void UpdateViewDistance(ivec2s currentChunk) // TODO(Aidan): Maybe find a way of not clearing the whole active chunks and instead just unrender the unneded ones
{
	VectorFreeivec2s(&activeChunks);
	VectorInitivec2s(&activeChunks);

	for (int x = currentChunk.x - viewDistance; x < currentChunk.x + viewDistance; x++)
	{
		for (int z = currentChunk.y - viewDistance; z < currentChunk.y + viewDistance; z++)
		{
			if (IsChunkInWorld(x, z))
			{
				if (dimensions[0].chunks[x][z] == 0)
				{
					dimensions[0].chunks[x][z] = malloc(sizeof(struct Chunk));
					MakeChunk(dimensions[0].chunks[x][z], x, 0, z);
					VectorPushBackivec2s(&updateMesh, (ivec2s){x, z});
				}
				VectorPushBackivec2s(&activeChunks, (ivec2s){x, z});
			}
		}
	}
}

void EditVoxel(struct Chunk *chunk, ivec3s position, uint8_t ID)
{
	if(IsVoxelInWorld(position.x, position.y, position.z) && IsVoxelInChunk(chunk, position.x, position.y, position.z) && chunk->renderable)
	{
		chunk->voxels[position.x][position.y][position.z] = ID;
		VectorPushBackivec2s(&modifyMesh, (ivec2s){chunk->x, chunk->z});

		// Update Surrounding Voxels
		ivec3s thisVoxel = (ivec3s){position.x, position.y, position.z};

		for (int i = 0; i < 4; i++) // Some useful info for the future. This has to be 6 when doing chunks that are on the y axis (vetically infiniate terrain) because right now its only checking if it needs to update the mesh of surround chunks on its xz axis. Same for similar things
		{
			ivec3s currentVoxel = glms_ivec3_add(thisVoxel, (ivec3s){wnormals[i][0], wnormals[i][1], wnormals[i][2]});
			if (!IsVoxelInChunk(chunk, currentVoxel.x, currentVoxel.y, currentVoxel.z) && dimensions[0].chunks[(int)floorf((float)(currentVoxel.x + chunk->position.x) / (float)CHUNK_SIZE_X)][(int)floorf((float)(currentVoxel.z + chunk->position.z) / (float)CHUNK_SIZE_Z)]->renderable)
				VectorPushBackivec2s(&modifyMesh, (ivec2s){(int)floorf((float)(currentVoxel.x + chunk->position.x) / (float)CHUNK_SIZE_X), (int)floorf((float)(currentVoxel.z + chunk->position.z) / (float)CHUNK_SIZE_Z)});
		}
	}
}

struct Chunk *GetChunk(int x, int z)
{
	if(IsChunkInWorld(x, z))
		return dimensions[0].chunks[x][z];
	else 
		return NULL;
}

uint8_t WorldGetVoxel(int x, int y, int z)
{
	int _x = (int)floorf((float)x / (float)CHUNK_SIZE_X);
	int _z = (int)floorf((float)z / (float)CHUNK_SIZE_Z);

	if(!IsVoxelInWorld(x, y, z))
		return 0;
	else
	{
		struct Chunk *currentChunk = dimensions[0].chunks[_x][_z];
		if(currentChunk == 0 || !currentChunk->populated)
			return GenerateVoxel((vec3s){_x * CHUNK_SIZE_X, 0, _z * CHUNK_SIZE_Z}, x - (_x * CHUNK_SIZE_X), y, z - (_z * CHUNK_SIZE_Z));
		else
			return GetVoxel(currentChunk, x - currentChunk->position.x, y - currentChunk->position.y, z - currentChunk->position.z);
	}
}

uint8_t GenerateVoxel(vec3s position, int x, int y, int z)
{
	// If bottom block of chunk, return bedrock.
	if (y == 0)
		return 4;

	/* BASIC TERRAIN PASS */

	struct Dimension currentDimension = dimensions[0];
	struct Biome currentBiome;
	int strongestBiomeIndex = 0;
	float strongestWeight = 0.0f;

	for (int i = 0; i < currentDimension.sizeofBiomes; i++)
	{
        float weight = Get2DSimplex(position.x + x, position.z + z, currentDimension.biomes[i].offset, currentDimension.biomes[i].scale); // Addint the x and z to the position makes the chunk borders not harsh on biome change and instead smooths it

        if (weight > strongestWeight)
		{
        	strongestWeight = weight;
        	strongestBiomeIndex = i;
		}
    }

	currentBiome = currentDimension.biomes[strongestBiomeIndex];

	int terrainHeight = currentBiome.terrainHeight + ((int)floor(20 * Get2DSimplex(x + position.x, z + position.z, currentBiome.terrainOffset, currentBiome.terrainScale)));
	uint8_t voxelValue = 0;
	for(int i = 0; i < currentBiome.sizeofBlocks; i++)
	{
		if(y >= terrainHeight - currentBiome.blocks[i].minHeight && y <= terrainHeight - currentBiome.blocks[i].maxHeight)
		{
			voxelValue = currentBiome.blocks[i].ID;
		}
	}

	for(int i = 0; i < currentBiome.sizeofLodes; i++)
	{
		if (voxelValue == currentBiome.lodes[i].replaceVoxel)
		{
			if (y >= terrainHeight - currentBiome.lodes[i].minHeight && y <= terrainHeight - currentBiome.lodes[i].maxHeight)
			{
				if (Get3DSimplex(position.x + x, position.y + y, position.z + z, currentBiome.lodes[i].offset, currentBiome.lodes[i].scale) > currentBiome.lodes[i].threshold) // TODO(Aidan): Check this out later
				{
					voxelValue = currentBiome.lodes[i].ID;
				}
			}
		}
	}

	for(int i = 0; i < currentBiome.sizeofStructures; i++)
	if(y == terrainHeight - currentBiome.structures[i].height)
	{
		if(Get2DSimplex(x + position.x, z + position.z, currentBiome.structures[i].offset, currentBiome.structures[i].scale) > currentBiome.structures[i].threshold)
		{
			if(Get2DSimplex(x + position.x, z + position.z, currentBiome.structures[i].subOffset, currentBiome.structures[i].subScale) > currentBiome.structures[i].subThreshold)
			{
				VectorPushBackvectorvoxelmod(&modifications, GenerateStructure(currentBiome.structures[i].type, (ivec3s){x + position.x, y + position.y, z + position.z}, currentBiome.structures[i].minHeight, currentBiome.structures[i].maxHeight,  currentBiome.structures[i].ID));
			}
		}
	}
	
	return voxelValue;
}

ivec2s GetCurrentChunkCoordinates(float x, float z)
{
	int _x = (int)floor(x / (float)CHUNK_SIZE_X);
	int _z = (int)floor(z / (float)CHUNK_SIZE_Z);

		return (ivec2s){_x, _z};
}

ivec3s GetBlockLookedAt(vec3s position, vec3s front, float reach, float increment, bool normalized)
{
	float step = increment;
	ivec3s lastPos = (ivec3s){0, 0, 0};

    while (step < reach)
    {
        vec3s pos = glms_vec3_add(position, glms_vec3_mul(front, (vec3s){step, step, step}));

        if (WorldGetVoxel((int)floorf(pos.x), (int)floorf(pos.y), (int)floorf(pos.z)) != 0)
			if(normalized)
				return (ivec3s){(int)floorf(lastPos.x), (int)floorf(lastPos.y), (int)(floorf(lastPos.z))};
			else
            	return (ivec3s){(int)floorf(pos.x), (int)floorf(pos.y), (int)(floorf(pos.z))};

		lastPos = (ivec3s){(int)floorf(pos.x), (int)floorf(pos.y), (int)(floorf(pos.z))};
        step += increment;
    }

	return (ivec3s){-1.0f, -1.0f, -1.0f}; // Nothing
}

bool IsVoxelInWorld(int x, int y, int z)
{
	if (x >= 0 && x < NUMBER_OF_CHUNKS_X * CHUNK_SIZE_X && y >= 0 && y < CHUNK_SIZE_Y && z >= 0 && z < NUMBER_OF_CHUNKS_X * CHUNK_SIZE_Z)
		return true;
	else
		return false;
}

bool IsChunkInWorld(int x, int z)
{
	if (x >= 0 && x < NUMBER_OF_CHUNKS_X && z >= 0 && z < NUMBER_OF_CHUNKS_Z)
		return true;
	else
		return false;
}