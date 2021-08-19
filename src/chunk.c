#include "chunk.h"

float verticeData[8][3] = 
{
 	{0, 0, 0},
	{1, 0, 0},
    {1, 1, 0},
    {0, 1, 0},
	{0, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 1, 1}
};

unsigned int indiceData[6][6] =
{
    {5, 6, 4, 7}, // Front Face
	{0, 3, 1, 2}, // Back Face
    {4, 7, 0, 3}, // Left Face
    {1, 2, 5, 6}, // Right Face
    {3, 7, 2, 6}, // Top Face
    {1, 5, 0, 4}  // Bottom Face
};

struct Block blocks[5] =
{
	{"Air", 0, -1, -1, -1, -1, -1, -1, true},
	{"Dirt", 1, 240, 240, 240, 240, 240, 240, false},
	{"Grass", 2, 241, 241, 241, 241, 242, 240, false},
	{"Stone", 3, 243, 243, 243, 243, 243, 243, false},
	{"Bedrock", 4, 244, 244, 244, 244, 244, 244, false}
};

struct Chunk MakeChunk(int x, int y, int z)
{
    struct Chunk chunk;
    chunk.VBO = 0;
    chunk.VAO = 0;
    chunk.EBO = 0;
    chunk.CBO = 0;
    chunk.UVBO = 0;
    chunk.indiceIndex = 0;

    chunk.position = (vec3s){(float)x * CHUNK_SIZE_X, (float)y * CHUNK_SIZE_Y, (float)z * CHUNK_SIZE_Z};

    return chunk;
}

void CreateChunk(struct Chunk *chunk)
{
    CreateChunkMesh(chunk);
    CreateChunkBufferData(chunk);
}

void CreateChunkMesh(struct Chunk *chunk)
{
    VectorfInit(&chunk->vertices);
    VectorfInit(&chunk->colors);
    VectorfInit(&chunk->uvs);
    VectoriInit(&chunk->indices);
	for(int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for(int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				if (y > CHUNK_SIZE_Y - 121)
					CreateVoxel(chunk, x, y, z, 0);
				else if(y == CHUNK_SIZE_Y - 121)
					CreateVoxel(chunk, x, y, z, 2);
				else if(y <= CHUNK_SIZE_Y - 122 && y >= CHUNK_SIZE_Y - 125)
					CreateVoxel(chunk, x, y, z, 1);
				else if(y == 0)
					CreateVoxel(chunk, x, y, z, 4);
				else
					CreateVoxel(chunk, x, y, z, 3);
			}
		}
	}
}

void CreateVoxel(struct Chunk *chunk, int x, int y, int z, uint8_t ID)
{
	if(!blocks[ID].isInvisible)
		{
		for(int i = 0; i < 6; i++)
		{
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][0]][0] + x);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][0]][1] + y);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][0]][2] + z);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][1]][0] + x);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][1]][1] + y);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][1]][2] + z);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][2]][0] + x);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][2]][1] + y);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][2]][2] + z);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][3]][0] + x);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][3]][1] + y);
			chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[indiceData[i][3]][2] + z);

			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);
			chunk->colors.pfVectorAdd(&chunk->colors, 1.0f);

			uint8_t textureID = 0;
			switch (i)
			{
			case 0:
				textureID = blocks[ID].frontFace;
				break;
			case 1:
				textureID = blocks[ID].backFace;
				break;
			case 2:
				textureID = blocks[ID].leftFace;
				break;
			case 3:
				textureID = blocks[ID].rightFace;
				break;
			case 4:
				textureID = blocks[ID].topFace;
				break;
			case 5:
				textureID = blocks[ID].bottomFace;
				break;
			
			default:
				break;
			}

			//printf("%i\n", (int)textureID);

			float uvy = (float)(textureID / TEXTURE_ATLAS_SIZE_IN_BLOCKS);
			float uvx = (float)(textureID - (uvy * TEXTURE_ATLAS_SIZE_IN_BLOCKS));

			float normalized = 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS;

			uvx *= normalized;
			uvy *= normalized;

			chunk->uvs.pfVectorAdd(&chunk->uvs, uvx + UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvy + UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvx + UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvy + normalized - UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvx + normalized - UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvy + UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvx + normalized - UV_OFFSET);
			chunk->uvs.pfVectorAdd(&chunk->uvs, uvy + normalized - UV_OFFSET);

			chunk->indices.pfVectorAdd(&chunk->indices, chunk->indiceIndex);
			chunk->indices.pfVectorAdd(&chunk->indices, chunk->indiceIndex + 1);
			chunk->indices.pfVectorAdd(&chunk->indices, chunk->indiceIndex + 2);
			chunk->indices.pfVectorAdd(&chunk->indices, chunk->indiceIndex + 2);
			chunk->indices.pfVectorAdd(&chunk->indices, chunk->indiceIndex + 1);
			chunk->indices.pfVectorAdd(&chunk->indices, chunk->indiceIndex + 3);
			chunk->indiceIndex += 4;
		}
	}
}

void CreateChunkBufferData(struct Chunk *chunk)
{
    glGenVertexArrays(1, &chunk->VAO);
	glGenBuffers(1, &chunk->VBO);
	glGenBuffers(1, &chunk->EBO);
	glGenBuffers(1, &chunk->CBO);
	glGenBuffers(1, &chunk->UVBO);

	glBindVertexArray(chunk->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
	glBufferData(GL_ARRAY_BUFFER, chunk->vertices.pfVectorTotal(&chunk->vertices) * sizeof(float), (const void *)chunk->vertices.vectorList.items, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->indices.pfVectorTotal(&chunk->indices) * sizeof(int), (const void *)chunk->indices.vectorList.items, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, chunk->CBO);
	glBufferData(GL_ARRAY_BUFFER, chunk->colors.pfVectorTotal(&chunk->colors) * sizeof(float), (const void *)chunk->colors.vectorList.items, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, chunk->UVBO);
	glBufferData(GL_ARRAY_BUFFER, chunk->uvs.pfVectorTotal(&chunk->uvs) * sizeof(float), (const void *)chunk->uvs.vectorList.items, GL_STATIC_DRAW);

	// position attribute
	glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glBindBuffer(GL_ARRAY_BUFFER, chunk->CBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glBindBuffer(GL_ARRAY_BUFFER, chunk->UVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
}

void DeleteChunk(struct Chunk *chunk)
{
    glDeleteVertexArrays(1, &chunk->VAO);
	glDeleteBuffers(1, &chunk->VBO);
	glDeleteBuffers(1, &chunk->EBO);
	glDeleteBuffers(1, &chunk->CBO);
	glDeleteBuffers(1, &chunk->UVBO);

	chunk->vertices.pfVectorFree(&chunk->vertices);
	chunk->indices.pfVectorFree(&chunk->indices);
	chunk->colors.pfVectorFree(&chunk->colors);
	chunk->uvs.pfVectorFree(&chunk->uvs);
}

void RenderChunk(struct Chunk *chunk, struct Shader *shader)
{
    mat4s model = { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f };

	model = glms_translate(model, chunk->position);
	SetShaderMatrix4(shader, "model", model, false);

	glBindVertexArray(chunk->VAO);
	glDrawElements(GL_TRIANGLES, chunk->indices.pfVectorTotal(&chunk->indices), GL_UNSIGNED_INT, 0);
}