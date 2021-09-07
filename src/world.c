#include "world.h"

defineFuntionsVector(ivec2s, ((ivec2s){0.0f, 0.0f}));

struct Chunk *chunks[NUMBER_OF_CHUNKS_X][NUMBER_OF_CHUNKS_Z] = {0};

int wnormals[6][3] = 
{
    { 0,  0,  1},
    { 0,  0, -1},
    {-1,  0,  0},
    { 1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0}
};

void WorldStart()
{
	if(viewDistance == 0)
		viewDistance = 4;
	
	VectorInitivec2s(&activeChunks);
	VectorInitivec2s(&updateMesh);
	VectorInitivec2s(&modifyMesh);
	for (int x = (NUMBER_OF_CHUNKS_X / 2) - viewDistance; x < (NUMBER_OF_CHUNKS_X / 2) + viewDistance; x++)
	{
		for (int z = (NUMBER_OF_CHUNKS_Z / 2) - viewDistance; z < (NUMBER_OF_CHUNKS_Z / 2) + viewDistance; z++)
		{
			chunks[x][z] =  malloc(sizeof(struct Chunk));
			MakeChunk(chunks[x][z], x, 0, z);
			CreateVoxels(chunks[x][z]);
		}
	}
	for (int x = (NUMBER_OF_CHUNKS_X / 2) - viewDistance; x < (NUMBER_OF_CHUNKS_X / 2) + viewDistance; x++)
	{
		for (int z = (NUMBER_OF_CHUNKS_Z / 2) - viewDistance; z < (NUMBER_OF_CHUNKS_Z / 2) + viewDistance; z++)
		{
			CreateVertices(chunks[x][z]);
			CreateChunkBufferData(chunks[x][z]);
			VectorPushBackivec2s(&activeChunks, (ivec2s){x, z});
		}
	}
}

void WorldRender(struct Texture2D *texture, struct Shader *shader)
{
	if(updateMesh.pfVectorTotalivec2s(&updateMesh) > 0)
	{
		CreateVoxels(chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y]);
		CreateVertices(chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y]);
	 	CreateChunkBufferData(chunks[VectorGetivec2s(&updateMesh, 0).x][VectorGetivec2s(&updateMesh, 0).y]);
		VectorDeleteivec2s(&updateMesh, 0);
	}
	while(modifyMesh.pfVectorTotalivec2s(&modifyMesh) > 0)
	{
		ClearChunk(chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]);
		CreateVertices(chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]);
	 	CreateChunkBufferData(chunks[VectorGetivec2s(&modifyMesh, 0).x][VectorGetivec2s(&modifyMesh, 0).y]);
		VectorDeleteivec2s(&modifyMesh, 0);
	}
	BindTexture(texture);
	for(int i = 0; i < activeChunks.pfVectorTotalivec2s(&activeChunks); i++)
		if(chunks[activeChunks.pfVectorGetivec2s(&activeChunks, i).x][activeChunks.pfVectorGetivec2s(&activeChunks, i).y]->renderable)
			RenderChunk(chunks[activeChunks.pfVectorGetivec2s(&activeChunks, i).x][activeChunks.pfVectorGetivec2s(&activeChunks, i).y], shader);
}

void WorldDelete()
{
	activeChunks.pfVectorFreeivec2s(&activeChunks);
	updateMesh.pfVectorFreeivec2s(&updateMesh);
	VectorFreeivec2s(&modifyMesh);
    for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
	{
		for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
		{
			DeleteChunk(chunks[x][z]);
		}
	}
}

void UpdateViewDistance(ivec2s currentChunk)
{
	VectorFreeivec2s(&activeChunks);
	VectorInitivec2s(&activeChunks);

	for (int x = currentChunk.x - viewDistance; x < currentChunk.x + viewDistance; x++)
	{
		for (int z = currentChunk.y - viewDistance; z < currentChunk.y + viewDistance; z++)
		{
			if (IsChunkInWorld(x, z))
			{
				if (chunks[x][z] == 0)
				{
					chunks[x][z] = malloc(sizeof(struct Chunk));
					MakeChunk(chunks[x][z], x, 0, z);
					VectorPushBackivec2s(&updateMesh, (ivec2s){x, z});
				}
				activeChunks.pfVectorAddivec2s(&activeChunks, (ivec2s){x, z});
			}
		}
	}
}

void BreakBlock(struct Chunk *chunk, ivec3s position)
{
	if(IsVoxelInWorld(position.x, position.y, position.z) && IsVoxelInChunk(chunk, position.x, position.y, position.z))
	{
		chunk->voxels[position.x][position.y][position.z] = 0;
		VectorPushBackivec2s(&modifyMesh, (ivec2s){chunk->x, chunk->z});

		// Update Surrounding Voxels
		ivec3s thisVoxel = (ivec3s){position.x, position.y, position.z};

		for (int i = 0; i < 6; i++)
		{
			ivec3s currentVoxel = glms_ivec3_add(thisVoxel, (ivec3s){wnormals[i][0], wnormals[i][1], wnormals[i][2]});
			if (!IsVoxelInChunk(chunk, currentVoxel.x, currentVoxel.y, currentVoxel.z))
				VectorPushBackivec2s(&modifyMesh, (ivec2s){(int)floorf((float)(currentVoxel.x + chunk->position.x) / (float)CHUNK_SIZE_X), (int)floorf((float)(currentVoxel.z + chunk->position.z) / (float)CHUNK_SIZE_Z)});
		}
	}
}

void PlaceBlock(struct Chunk *chunk, ivec3s position, uint8_t ID)
{
	if(IsVoxelInWorld(position.x, position.y, position.z) && IsVoxelInChunk(chunk, position.x, position.y, position.z))
	{
		chunk->voxels[position.x][position.y][position.z] = ID;
		VectorPushBackivec2s(&modifyMesh, (ivec2s){chunk->x, chunk->z});

		// Update Surrounding Voxels
		ivec3s thisVoxel = (ivec3s){position.x, position.y, position.z};

		for (int i = 0; i < 6; i++)
		{
			ivec3s currentVoxel = glms_ivec3_add(thisVoxel, (ivec3s){wnormals[i][0], wnormals[i][1], wnormals[i][2]});
			if (!IsVoxelInChunk(chunk, currentVoxel.x, currentVoxel.y, currentVoxel.z))
				VectorPushBackivec2s(&modifyMesh, (ivec2s){(int)floorf((float)(currentVoxel.x + chunk->position.x) / (float)CHUNK_SIZE_X), (int)floorf((float)(currentVoxel.z + chunk->position.z) / (float)CHUNK_SIZE_Z)});
		}
	}
}

struct Chunk *GetChunk(int x, int z)
{
	if(IsChunkInWorld(x, z))
		return chunks[x][z];
}

uint8_t WorldGetVoxel(int x, int y, int z)
{
	int _x = (int)floorf((float)x / (float)CHUNK_SIZE_X);
	int _z = (int)floorf((float)z / (float)CHUNK_SIZE_Z);

	if(!IsVoxelInWorld(x, y, z))
		return 0;
	else
	{
		struct Chunk *currentChunk = chunks[_x][_z];
		if(currentChunk == 0 || !currentChunk->populated)
			return GenerateVoxel((vec3s){_x * CHUNK_SIZE_X, 0, _z * CHUNK_SIZE_Z}, x - (_x * CHUNK_SIZE_X), y, z - (_z * CHUNK_SIZE_Z));
		else
			return GetVoxel(currentChunk, x - currentChunk->position.x, y - currentChunk->position.y, z - currentChunk->position.z);
	}
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