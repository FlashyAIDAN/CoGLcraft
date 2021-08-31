#include "world.h"

//defineFuntionsVector(ivec2s, ((ivec2s){0.0f, 0.0f}));

void VectorInitivec2s(vectorivec2s *v)
{ 
v->pfVectorTotalivec2s = VectorTotalivec2s;
v->pfVectorResizeivec2s = VectorResizeivec2s;
v->pfVectorAddivec2s = VectorPushBackivec2s;
v->pfVectorSetivec2s = VectorSetivec2s;
v->pfVectorGetivec2s = VectorGetivec2s;
v->pfVectorFreeivec2s = VectorFreeivec2s;
v->pfVectorDeleteivec2s = VectorDeleteivec2s;
v->vectorListivec2s.capacity = 1;
v->vectorListivec2s.total = 0;
v->vectorListivec2s.items = malloc(sizeof(ivec2s) * v->vectorListivec2s.capacity);
}

int VectorTotalivec2s(vectorivec2s *v) 
{ 
	int totalCount = -1;
	if(v) totalCount = v->vectorListivec2s.total;
	return totalCount; 
}

int VectorResizeivec2s(vectorivec2s *v, int capacity) 
{ 
	int status = -1;
	if(v)
	{ 
		ivec2s *items = realloc(v->vectorListivec2s.items, sizeof(ivec2s) * capacity);
		if (items)
		{ 
			v->vectorListivec2s.items = items;
			v->vectorListivec2s.capacity = capacity;
			status = 0;
		} 
	}
	return status;
} 

int VectorPushBackivec2s(vectorivec2s *v, ivec2s item)
{
	int status = -1;
	if(v) 
	{
		if(v->vectorListivec2s.capacity == v->vectorListivec2s.total)
		{
			status = VectorResizeivec2s(v, v->vectorListivec2s.capacity * 2);
			if(status != -1)
				v->vectorListivec2s.items[v->vectorListivec2s.total++] = item;
		}
		else
		{
			v->vectorListivec2s.items[v->vectorListivec2s.total++] = item;
			status = 0;
		}
	}
	return status;
}

int VectorSetivec2s(vectorivec2s *v, int index, ivec2s item)
{
	int status = -1; 
	if(v) 
	{ 
		if((index >= 0) && (index < v->vectorListivec2s.total)) 
		{
			v->vectorListivec2s.items[index] = item;
			status = 0; 
		} 
	} 
	return status; 
} 

ivec2s VectorGetivec2s(vectorivec2s *v, int index) 
{ 
	ivec2s readData = ((ivec2s){0.0f, 0.0f}); 
	if(v) 
	{ 
		if((index >= 0) && (index < v->vectorListivec2s.total)) 
			readData = v->vectorListivec2s.items[index]; 
	} 
	return readData; 
}

int VectorDeleteivec2s(vectorivec2s *v, int index) 
{ 
	int status = -1; 
	int i = 0; 
	if(v) 
	{ 
		if((index < 0) || (index >= v->vectorListivec2s.total)) 
			return status; 
		v->vectorListivec2s.items[index] = ((ivec2s){0.0f, 0.0f}); 
		for(i = index; (i < v->vectorListivec2s.total - 1); ++i) 
		{ 
			v->vectorListivec2s.items[i] = v->vectorListivec2s.items[i + 1]; 
			v->vectorListivec2s.items[i + 1] = ((ivec2s){0.0f, 0.0f});
		} 
		v->vectorListivec2s.total--; 
		if((v->vectorListivec2s.total > 0) && ((v->vectorListivec2s.total) == (v->vectorListivec2s.capacity / 4)))
			VectorResizeivec2s(v, v->vectorListivec2s.capacity / 2);
		status = 0; 
	} 
	return status; 
} 

int VectorFreeivec2s(vectorivec2s *v) 
{ 
	int status = -1; 
	if(v) 
	{ 
		free(v->vectorListivec2s.items); 
		v->vectorListivec2s.items = ((void *)0); 
		status = 0; 
	} 
	return status; 
}

vectorivec2s activeChunks;
vectorivec2s testUpdateMesh;

int viewDistance = 4;

void WorldStart()
{
	VectorInitivec2s(&activeChunks);
	VectorInitivec2s(&testUpdateMesh);
	for(int x = 0; x < viewDistance; x++)
	{
		for(int z = 0; z < viewDistance; z++)
		{
			chunks[x + (NUMBER_OF_CHUNKS_X / 2)][z + (NUMBER_OF_CHUNKS_Z / 2)] = MakeChunk(x + (NUMBER_OF_CHUNKS_X / 2), 0, z + (NUMBER_OF_CHUNKS_Z / 2));
			CreateVoxels(&chunks[x + (NUMBER_OF_CHUNKS_X / 2)][z + (NUMBER_OF_CHUNKS_Z / 2)]);
		}
	}
	for(int x = 0; x < viewDistance; x++)
	{
		for(int z = 0; z < viewDistance; z++)
		{
			CreateVertices(&chunks[x + (NUMBER_OF_CHUNKS_X / 2)][z + (NUMBER_OF_CHUNKS_Z / 2)]);
			CreateChunkBufferData(&chunks[x + (NUMBER_OF_CHUNKS_X / 2)][z + (NUMBER_OF_CHUNKS_Z / 2)]);
			VectorPushBackivec2s(&activeChunks, (ivec2s){x + (NUMBER_OF_CHUNKS_X / 2), z + (NUMBER_OF_CHUNKS_Z / 2)});
		}
	}
}

void WorldRender(struct Texture2D *texture, struct Shader *shader)
{
	BindTexture(texture);
	for(int i = 0; i < activeChunks.pfVectorTotalivec2s(&activeChunks); i++)
		RenderChunk(&chunks[activeChunks.pfVectorGetivec2s(&activeChunks, i).x][activeChunks.pfVectorGetivec2s(&activeChunks, i).y], shader);
}

void WorldDelete()
{
    for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
	{
		for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
		{
			DeleteChunk(&chunks[x][z]);
		}
	}
}

void UpdateViewDistance(ivec2s currentChunk)
{
	//VectorFreeivec2s(&activeChunks);

	for (int x = currentChunk.x - viewDistance; x < currentChunk.x + viewDistance; x++)
	{
		for (int z = currentChunk.y - viewDistance; z < currentChunk.y + viewDistance; z++)
		{
			if (IsChunkInWorld(x, z))
			{
				if ((uint8_t)&chunks[x][z] == 0)
				{
					CreateVoxels(&chunks[x][z]);
					VectorPushBackivec2s(&testUpdateMesh, (ivec2s){x, z});
					//activeChunks.pfVectorAddivec2s(&activeChunks, (ivec2s){x, z});
				}
				//activeChunks.pfVectorAddivec2s(&activeChunks, (ivec2s){x, z});
			}
		}
	}

	for(int i = 0; i < testUpdateMesh.pfVectorTotalivec2s(&testUpdateMesh); i++)
	{
		CreateVertices(&chunks[testUpdateMesh.pfVectorGetivec2s(&testUpdateMesh, i).x][testUpdateMesh.pfVectorGetivec2s(&testUpdateMesh, i).y]);
		CreateChunkBufferData(&chunks[testUpdateMesh.pfVectorGetivec2s(&testUpdateMesh, i).x][testUpdateMesh.pfVectorGetivec2s(&testUpdateMesh, i).y]);
		testUpdateMesh.pfVectorDeleteivec2s(&testUpdateMesh, i);
	}
	testUpdateMesh.pfVectorFreeivec2s(&testUpdateMesh);
}

uint8_t WorldGetVoxel(int x, int y, int z)
{
	int _x = (int)floor((float)x / (float)CHUNK_SIZE_X);
	int _z = (int)floor((float)z / (float)CHUNK_SIZE_Z);

	if(!IsVoxelInWorld(x, y, z))
		return 0;
	else
	{
		struct Chunk currentChunk = chunks[_x][_z];
		if((uint8_t)&currentChunk == 0)
			return 0;
		else
			return GetVoxel(&currentChunk, x - currentChunk.position.x, y - currentChunk.position.y, z - currentChunk.position.z);
	}
}

ivec2s GetCurrentChunkCoordinates(float x, float z)
{
	int _x = (int)floor(x / (float)CHUNK_SIZE_X);
	int _z = (int)floor(z / (float)CHUNK_SIZE_Z);

	if(!IsChunkInWorld(_x, _z))
		return (ivec2s){0, 0}; // Just in case, but will cause wird glitch and the player will use the first chunk
	else
		return (ivec2s){_x, _z};
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