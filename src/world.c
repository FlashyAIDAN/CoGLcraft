#include "world.h"

void WorldStart()
{
	for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
	{
		for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
		{
			chunks[x][z] = MakeChunk(x, 0, z);
			CreateVoxels(&chunks[x][z]);
		}
	}
	for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
	{
		for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
		{
			CreateVertices(&chunks[x][z]);
			CreateChunkBufferData(&chunks[x][z]);
		}
	}
}

void WorldRender(struct Texture2D *texture, struct Shader *shader)
{
	BindTexture(texture);
	for(int x = 0; x < NUMBER_OF_CHUNKS_X; x++)
	{
		for(int z = 0; z < NUMBER_OF_CHUNKS_Z; z++)
		{
				RenderChunk(&chunks[x][z], shader);
		}
	}
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

uint8_t WorldGetVoxel(int x, int y, int z)
{
	int _x = (int)floor((float)x / (float)CHUNK_SIZE_X);
	int _z = (int)floor((float)z / (float)CHUNK_SIZE_Z);

	if(_x < 0 || _x > NUMBER_OF_CHUNKS_X - 1 || _z < 0 || _z > NUMBER_OF_CHUNKS_Z - 1 || y < 0 || y > CHUNK_SIZE_Y - 1)
		return 0;
	else
	{
		struct Chunk currentChunk = chunks[_x][_z];
		return GetVoxel(&currentChunk, x - currentChunk.position.x, y - currentChunk.position.y, z - currentChunk.position.z);
	}
}