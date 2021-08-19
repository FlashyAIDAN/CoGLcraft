#include "chunk.h"

float verticeData[12][3] = 
{
 	{ 1.0f, 1.0f, 0.0f },
 	{ 1.0f, 0.0f, 0.0f },
 	{ 0.0f, 0.0f, 0.0f },
 	{ 0.0f, 1.0f, 0.0f },
 	{ 1.0f, 1.0f, 1.0f },
 	{ 1.0f, 0.0f, 1.0f },
 	{ 0.0f, 0.0f, 1.0f },
 	{ 0.0f, 1.0f, 1.0f },
 	{ 0.0f, 1.0f, 1.0f },
 	{ 1.0f, 0.0f, 1.0f },
 	{ 1.0f, 1.0f, 1.0f }, 
 	{ 0.0f, 0.0f, 1.0f }
};

float uvData[12][2] =
{
	{ 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET, 	1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET },
 	{ 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET, 	0.0f + UV_OFFSET },
 	{ 0.0f + UV_OFFSET, 								0.0f + UV_OFFSET },
 	{ 0.0f + UV_OFFSET, 								1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET },
 	{ 0.0f + UV_OFFSET, 								1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET },
 	{ 0.0f + UV_OFFSET, 								0.0f + UV_OFFSET },
	{ 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET, 	0.0f + UV_OFFSET },
	{ 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET, 	1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET },
	{ 0.0f + UV_OFFSET,									0.0f + UV_OFFSET },
	{ 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET,  1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET },
	{ 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET,  0.0f + UV_OFFSET },
	{ 0.0f + UV_OFFSET, 								1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS - UV_OFFSET }
};

unsigned int indiceData[6][6] =
{
	{ 7, 6, 5,  7, 5,  4 }, // FRONT
	{ 0, 1, 3,  1, 2,  3 }, // BACK
	{ 2, 6, 7,  7, 3,  2 }, // LEFT
	{ 1, 4, 5,  4, 1,  0 }, // RIGHT
	{ 0, 3, 10, 8, 10, 3 }, // TOP
	{ 2, 9, 11, 9, 2,  1 }  // BOTTOM
};

struct Block blocks[2] =
{
	{0, 240, 240, 240, 241, 240, 240},
	{1, 241, 243, 241, 243, 242, 240}	
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
				CreateVoxel(chunk, x, y, z, 0);
			}
		}
	}
}

void CreateVoxel(struct Chunk *chunk, int x, int y, int z, uint8_t ID)
{
    int j = 0;
	for(int i = 0; i < 6; i++)
	{
		chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[i + j][0] + x);
		chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[i + j][1] + y);
		chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[i + j][2] + z);
		chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[i + j + 1][0] + x);
		chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[i + j + 1][1] + y);
		chunk->vertices.pfVectorAdd(&chunk->vertices, verticeData[i + j + 1][2] + z);

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

		uvx *= 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS;
		uvy *= 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS;

		chunk->uvs.pfVectorAdd(&chunk->uvs, uvx + uvData[i + j][0]);
		chunk->uvs.pfVectorAdd(&chunk->uvs, uvy + uvData[i + j][1]);
		chunk->uvs.pfVectorAdd(&chunk->uvs, uvx + uvData[i + j + 1][0]);
		chunk->uvs.pfVectorAdd(&chunk->uvs, uvy + uvData[i + j + 1][1]);

		chunk->indices.pfVectorAdd(&chunk->indices, indiceData[i][0] + chunk->indiceIndex);
		chunk->indices.pfVectorAdd(&chunk->indices, indiceData[i][1] + chunk->indiceIndex);
		chunk->indices.pfVectorAdd(&chunk->indices, indiceData[i][2] + chunk->indiceIndex);
		chunk->indices.pfVectorAdd(&chunk->indices, indiceData[i][3] + chunk->indiceIndex);
		chunk->indices.pfVectorAdd(&chunk->indices, indiceData[i][4] + chunk->indiceIndex);
		chunk->indices.pfVectorAdd(&chunk->indices, indiceData[i][5] + chunk->indiceIndex);
		j += 1;
	}
	chunk->indiceIndex += 12;
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