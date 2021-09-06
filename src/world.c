#include "world.h"

defineFuntionsVector(ivec2s, ((ivec2s){0.0f, 0.0f}));

struct Chunk *chunks[NUMBER_OF_CHUNKS_X][NUMBER_OF_CHUNKS_Z] = {0};

int viewDistance = 4;

void WorldStart()
{
	VectorInitivec2s(&activeChunks);
	VectorInitivec2s(&testUpdateMesh);
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
	BindTexture(texture);
	for(int i = 0; i < activeChunks.pfVectorTotalivec2s(&activeChunks); i++)
		RenderChunk(chunks[activeChunks.pfVectorGetivec2s(&activeChunks, i).x][activeChunks.pfVectorGetivec2s(&activeChunks, i).y], shader);
}

void WorldDelete()
{
	activeChunks.pfVectorFreeivec2s(&activeChunks);
	testUpdateMesh.pfVectorFreeivec2s(&testUpdateMesh);
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
	VectorInitivec2s(&testUpdateMesh);

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
					CreateVoxels(chunks[x][z]);
					VectorPushBackivec2s(&testUpdateMesh, (ivec2s){x, z});
				}
				activeChunks.pfVectorAddivec2s(&activeChunks, (ivec2s){x, z});
			}
		}
	}

	for(int i = 0; i < testUpdateMesh.pfVectorTotalivec2s(&testUpdateMesh); i++)
	{
		CreateVertices(chunks[VectorGetivec2s(&testUpdateMesh, i).x][VectorGetivec2s(&testUpdateMesh, i).y]);
		CreateChunkBufferData(chunks[VectorGetivec2s(&testUpdateMesh, i).x][VectorGetivec2s(&testUpdateMesh, i).y]);
	}
	VectorFreeivec2s(&testUpdateMesh);
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
		if(currentChunk == 0)
			return 0;
		else
			return GetVoxel(currentChunk, x - currentChunk->position.x, y - currentChunk->position.y, z - currentChunk->position.z);
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