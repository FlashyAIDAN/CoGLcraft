#include "world.h"

defineFuntionsVector(ivec2s, ((ivec2s){0.0f, 0.0f}));
defineFuntionsVector(ivec3s, ((ivec3s){0.0f, 0.0f, 0.0f}));
defineFuntionsVector(vectorvoxelmod, ((vectorvoxelmod){0}));

struct Lode lodes[3] =
{
	{"Iron Ore", 12, 3, 256, 30, 0.0f, 0.25f, 0.7f},
	{"Coal Ore", 13, 3, 256, 0, 500.0f, 0.125f, 0.75f},
	{"Basic Cave", 0, 3, 100, 60, 1250.0f, 0.075f, 0.5f}
};

struct Biome biomes[2] =
{
	{"Forest", 100, 0.0f, 0.125f, 0.0f, 0.25f, NULL, NULL, NULL, 3, 3, 3},
	{"Desert", 100, 500.0f, 0.125f, 0.0f, 0.25f, NULL, NULL, NULL, 1, 1, 1}
	// {"Mountain", 125, 875.0f, 0.075f, 0.0f, 0.125f, NULL, NULL, NULL, 1, 0, 3}
};

struct Dimension dimensions[1] =
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

void WorldStart(struct Shader *shader, ivec2s currentChunk)
{
	// BIOME 1
	biomes[0].blocks = calloc(biomes[0].sizeofBlocks, sizeof(struct BiomeBlock));
	biomes[0].structures = calloc(biomes[0].sizeofStructures, sizeof(struct Structure));
	biomes[0].lodes = calloc(biomes[0].sizeofLodes, sizeof(struct Lode));
	biomes[0].blocks[0] = (struct BiomeBlock){2, 0, 0};
	biomes[0].blocks[1] = (struct BiomeBlock){1, 5, 1};
	biomes[0].blocks[2] = (struct BiomeBlock){3, 256, 6};
	biomes[0].structures[0] = (struct Structure){SMALLHOUSE, 4, 7, 0, 0, 1250.0f, 3.3f, 0.6f, 0.0f, 15.0f, 0.7f};
	biomes[0].structures[1] = (struct Structure){TREE, 4, 7, 0, 0, 0.0f, 1.3f, 0.6f, 0.0f, 15.0f, 0.5f};
	biomes[0].structures[2] = (struct Structure){PLANT, 0, 0, 0, 18, 1000.0f, 2.0f, 0.3f, 0.0f, 15.0f, 0.5f}; // Make sure this is first or else they will replace the base trunk of trees ifd they overlap
	biomes[0].lodes[0] = lodes[0];
	biomes[0].lodes[1] = lodes[1];
	biomes[0].lodes[2] = lodes[2];

	// BIOME 2
	biomes[1].blocks = calloc(biomes[1].sizeofBlocks, sizeof(struct BiomeBlock));
	biomes[1].structures = calloc(biomes[1].sizeofStructures, sizeof(struct Structure));
	biomes[1].lodes = calloc(biomes[1].sizeofLodes, sizeof(struct Lode));
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
	// biomes[2].lodes[1] = lodes[2];

	dimensions[0].biomes = calloc(2, sizeof(struct Biome));
	dimensions[0].biomes[0] = biomes[0];
	dimensions[0].biomes[1] = biomes[1];
	//dimensions[0].biomes[2] = biomes[2];

	if(viewDistance == 0)
		viewDistance = 4;

	SetShaderFloat(shader, "fogNear", viewDistance * CHUNK_SIZE_X - 25.0f, true);
	SetShaderFloat(shader, "fogFar", viewDistance * CHUNK_SIZE_X, false);
	
	VectorInitivec2s(&activeChunks);
	VectorInitivec2s(&updateMesh);
	VectorInitivec2s(&modifyMesh);
	VectorInitivec2s(&makeMesh);

	VectorInitvectorvoxelmod(&modifications);

	VECTOR_INIT(ivec3s, startMesh);
	for(unsigned int i = 0; i < (sizeof(dimensions) / sizeof(dimensions[0])); i++)
	{
		for(unsigned int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
		{
			for(unsigned int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
			{
				FILE *inf;
				char s[10000]; // This is not perminent an will eventually run out of char space, once x gets too big
				sprintf(s, "res/saves/main/chunks/%s/x%dz%d.chunk", dimensions[i].name, x, z); // Folders need to be created automatically
				inf = fopen (s, "r");
				if (inf == NULL)
					goto skip;
				dimensions[i].chunks[x][z] = malloc(sizeof(struct Chunk));
				MakeChunk(dimensions[i].chunks[x][z], x, 0, z);
				for(unsigned int cx = 0; cx < CHUNK_SIZE_X; cx++)
				{
					for(unsigned int cy = 0; cy < CHUNK_SIZE_Y; cy++)
					{
						for(unsigned int cz = 0; cz < CHUNK_SIZE_Z; cz++)
						{
							fread(&dimensions[i].chunks[x][z]->voxels[cx][cy][cz], sizeof(uint8_t), 1, inf);
						}
					}
				}
				dimensions[i].chunks[x][z]->populated = true;
				if(IsChunkInWorld((ivec2s){x, z}))
				{
					if((x >= currentChunk.x - viewDistance && x <= currentChunk.x + viewDistance) && (z >= currentChunk.y - viewDistance && z <= currentChunk.y + viewDistance))
					{
						VectorPushBackivec3s(&startMesh, (ivec3s){i, x, z});
						VectorPushBackivec2s(&activeChunks, (ivec2s){x, z});
					}
				}
				
				fclose(inf);
				skip:;
			}
		}
	}

	for(int i = 0; i < VectorTotalivec3s(&startMesh); i++)
	{
		CreateVertices(dimensions[VectorGetivec3s(&startMesh, i).x].chunks[VectorGetivec3s(&startMesh, i).y][VectorGetivec3s(&startMesh, i).z]);
		//CreateChunkBufferData(dimensions[VectorGetivec3s(&startMesh, i).x].chunks[VectorGetivec3s(&startMesh, i).y][VectorGetivec3s(&startMesh, i).z]);
	}
	VectorFreeivec3s(&startMesh);

	for (unsigned int x = currentChunk.x - viewDistance; x < currentChunk.x + viewDistance; x++)
	{
		for (unsigned int z = currentChunk.y - viewDistance; z < currentChunk.y + viewDistance; z++)
		{
			if(IsChunkInWorld((ivec2s){x, z}))
			{
				if(dimensions[0].chunks[x][z] == 0 || !dimensions[0].chunks[x][z]->renderable)
				{
					dimensions[0].chunks[x][z] = malloc(sizeof(struct Chunk));
					MakeChunk(dimensions[0].chunks[x][z], x, 0, z);
					VectorPushBackivec2s(&updateMesh, (ivec2s){x, z});
					VectorPushBackivec2s(&activeChunks, (ivec2s){x, z});
				}
			}
		}
	}
}

void WorldRender(struct Texture2D *texture, struct Shader *shader)
{
	if(VectorTotalivec2s(&updateMesh) > 0)
	{
		struct Chunk *currentChunk = dimensions[0].chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y];
		currentChunk->inVector = false;
		CreateVoxels(currentChunk);
		int e = 0;
		int f = 0;
		while (VectorTotalvectorvoxelmod(&modifications) > e) // FIX THE > e and > f
		{
			vectorvoxelmod queue = VectorGetvectorvoxelmod(&modifications, e);
			while (VectorTotalvoxelmod(&queue) > f)
			{
				struct VoxelMod voxel = VectorGetvoxelmod(&queue, f);

				if(IsChunkInWorld((ivec2s){(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)}))
				{
					struct Chunk *currentChunkMod = dimensions[0].chunks[(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X)][(int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)];
					if (currentChunkMod == 0)
					{
						currentChunkMod = malloc(sizeof(struct Chunk));
						MakeChunk(currentChunkMod, (int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), 0, (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z));
					}
					else if(!currentChunkMod->modified && currentChunkMod->renderable)
					{
						VectorPushBackivec2s(&modifyMesh, (ivec2s){(int)floorf((int)voxel.position.x / (float)CHUNK_SIZE_X), (int)floorf((int)voxel.position.z / (float)CHUNK_SIZE_Z)});
						currentChunkMod->modified = true;
					}
					VectorPushBackvoxelmod(&currentChunkMod->modifications, voxel);
				}
				f++;
			}
			f = 0;
			e++;
		}
		VectorFreevectorvoxelmod(&modifications);
		VectorInitvectorvoxelmod(&modifications);
		
		CreateVertices(currentChunk);
	 	//CreateChunkBufferData(currentChunk);
		VectorDeleteivec2s(&updateMesh, 0);
	}
	else if(VectorTotalivec2s(&makeMesh) > 0)
	{
		struct Chunk *currentChunk = dimensions[0].chunks[VectorGetivec2s(&makeMesh, 0).x][VectorGetivec2s(&makeMesh, 0).y];
		currentChunk->inVector = false;
		CreateVertices(currentChunk);
	 	//CreateChunkBufferData(currentChunk);
		VectorDeleteivec2s(&makeMesh, 0);
	}
	int d = 0;
	while(VectorTotalivec2s(&modifyMesh) > d)
	{
		struct Chunk *currentChunk = dimensions[0].chunks[VectorGetivec2s(&modifyMesh, d).x][VectorGetivec2s(&modifyMesh, d).y];
		ClearChunk(currentChunk);
		CreateVertices(currentChunk);
	 	//CreateChunkBufferData(currentChunk);
		currentChunk->modified = false;
		d++;
	}
	VectorFreeivec2s(&modifyMesh);
	VectorInitivec2s(&modifyMesh);
	
	BindTexture(texture);
	for(int i = 0; i < VectorTotalivec2s(&activeChunks); i++)
		if(dimensions[0].chunks[VectorGetivec2s(&activeChunks, i).x][VectorGetivec2s(&activeChunks, i).y]->renderable)
			RenderChunk(dimensions[0].chunks[VectorGetivec2s(&activeChunks, i).x][VectorGetivec2s(&activeChunks, i).y], shader);
}

void WorldDelete()
{
	VectorFreeivec2s(&updateMesh);
	VectorFreeivec2s(&modifyMesh);
	VectorFreeivec2s(&makeMesh);
	VectorFreeivec2s(&activeChunks);
	VectorFreevectorvoxelmod(&modifications);

	int written = 0; // TODO(Aidan):Read and Write Chunk Data From file

	for(unsigned int i = 0; i < (sizeof(dimensions) / sizeof(dimensions[0])); i++)
	{
		for(unsigned int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
		{
			for(unsigned int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
			{
				if(dimensions[i].chunks[x][z] != 0 && dimensions[i].chunks[x][z]->renderable && dimensions[i].chunks[x][z]->populated)
				{
					char s[10000]; // This is not perminent an will eventually run out of char space, once x gets too big
					sprintf(s, "res/saves/main/chunks/%s/x%dz%d.chunk", dimensions[i].name, x, z); // Folders need to be created automatically
					FILE *f = fopen(s, "w");
					if (f == NULL)
      					printf("/nError to open the file!\n");
					for(unsigned int cx = 0; cx < CHUNK_SIZE_X; cx++)
					{
						for(unsigned int cy = 0; cy < CHUNK_SIZE_Y; cy++)
						{
							for(unsigned int cz = 0; cz < CHUNK_SIZE_Z; cz++)
							{
								written = fwrite(&dimensions[i].chunks[x][z]->voxels[cx][cy][cz], sizeof(uint8_t), 1, f);
								if (written == 0)
									printf("Error during writing to file!\n");
							}
						}
					}

					fclose(f);
				}
			}
		}
	}
	
	for(int i = 0; i < (sizeof(dimensions) / sizeof(dimensions[0])); i++)
	{
		for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
		{
			for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
			{
				if(dimensions[i].chunks[x][z] != 0)
				{
					DeleteChunk(dimensions[i].chunks[x][z]);
				}
			}
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
			if (IsChunkInWorld((ivec2s){x, z}))
			{
				if (dimensions[0].chunks[x][z] == 0)
				{
					dimensions[0].chunks[x][z] = malloc(sizeof(struct Chunk));
					MakeChunk(dimensions[0].chunks[x][z], x, 0, z);
					VectorPushBackivec2s(&updateMesh, (ivec2s){x, z});
				}
				else if(dimensions[0].chunks[x][z]->populated && !dimensions[0].chunks[x][z]->renderable && !dimensions[0].chunks[x][z]->inVector)
				{
					dimensions[0].chunks[x][z]->inVector = true;
					VectorPushBackivec2s(&makeMesh, (ivec2s){x, z});
				}
				else if(!dimensions[0].chunks[x][z]->populated && !dimensions[0].chunks[x][z]->renderable && !dimensions[0].chunks[x][z]->inVector)
				{
					dimensions[0].chunks[x][z]->inVector = true;
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
		VectorPushBackivec2s(&modifyMesh, (ivec2s){(int)floorf(chunk->position.x / CHUNK_SIZE_X), (int)floorf(chunk->position.z / CHUNK_SIZE_Z)});

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

struct Chunk *GetChunk(ivec2s position)
{
	if(IsChunkInWorld((ivec2s){position.x, position.y}))
		return dimensions[0].chunks[position.x][position.y];
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

ivec2s GetCurrentChunkCoordinates(vec2s position)
{
	int _x = (int)floorf(position.x / (float)CHUNK_SIZE_X);
	int _z = (int)floorf(position.y / (float)CHUNK_SIZE_Z);

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

bool IsChunkInWorld(ivec2s position)
{
	if (position.x >= 0 && position.x < NUMBER_OF_CHUNKS_X && position.y >= 0 && position.y < NUMBER_OF_CHUNKS_Z)
		return true;
	else
		return false;
}