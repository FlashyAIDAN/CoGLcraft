#include "chunk.h"
#include "world.h"

defineFuntionsVector(float, 0);
defineFuntionsVector(int, 0);

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

unsigned int indiceData[6][4] =
{
    {5, 6, 4, 7}, // Front Face
	{0, 3, 1, 2}, // Back Face
    {4, 7, 0, 3}, // Left Face
    {1, 2, 5, 6}, // Right Face
    {3, 7, 2, 6}, // Top Face
    {1, 5, 0, 4}  // Bottom Face
};

int normals[6][3] = 
{
    { 0,  0,  1},
    { 0,  0, -1},
    {-1,  0,  0},
    { 1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0}
};

struct Voxel voxels[8] =
{
	{"Air", -1, -1, -1, -1, -1, -1, true, true},
	{"Dirt", 242, 242, 242, 242, 242, 242, false, false},
	{"Grass", 243, 243, 243, 243, 240, 242, false, false},
	{"Stone", 241, 241, 241, 241, 241, 241, false, false},
	{"Bedrock", 225, 225, 225, 225, 225, 225, false, false},
	{"Log", 228, 228, 228, 228, 229, 229, false, false},
	{"Leaf", 196, 196, 196, 196, 196, 196, false, true},
	{"Glass", 193, 193, 193, 193, 193, 193, false, true}
};

void MakeChunk(struct Chunk *chunk, int x, int y, int z)
{
	memset(chunk, 0, sizeof(struct Chunk));
	chunk->x = x;
	chunk->z = z;
	
    chunk->VBO = 0;
    chunk->VAO = 0;
    chunk->EBO = 0;
    chunk->CBO = 0;
    chunk->UVBO = 0;
    chunk->indiceIndex = 0;

    chunk->position = (vec3s){(float)x * CHUNK_SIZE_X, (float)y * CHUNK_SIZE_Y, (float)z * CHUNK_SIZE_Z};

	chunk->renderable = false;
	chunk->populated = false;
}

uint8_t GenerateVoxel(vec3s position, int x, int y, int z)
{
	//printf("%i\n",(int)floor(42 * Get2DPerlin(x, z, 0, 0.25)) + 42);
	// If bottom block of chunk, return bedrock.
	if (y == 0)
		return 4;

	/* BASIC TERRAIN PASS */

	int terrainHeight = 100 + ((int)floor(20 * Get2DSimplex(position.x + x, position.z + z, 0, 0.25)));
	uint8_t voxelValue = 0;

	if (y == terrainHeight)
		voxelValue =  2;
	else if (y < terrainHeight && y > terrainHeight - 4)
		voxelValue = 1;
	else if (y > terrainHeight)
		return 0;
	else
		voxelValue = 3;
	
	return voxelValue;
}

uint8_t GetVoxel(struct Chunk *chunk, int x, int y, int z)
{
	if(!IsVoxelInChunk(chunk, x, y, z))
		return WorldGetVoxel(x + chunk->position.x, y + chunk->position.y, z + chunk->position.z);
	else
		return chunk->voxels[x][y][z];
}

void CreateVoxel(struct Chunk *chunk, int x, int y, int z, uint8_t ID)
{
	if(!voxels[ID].isInvisible)
	{
		for(int i = 0; i < 6; i++)
		{
				if(voxels[GetVoxel(chunk, x + normals[i][0], y + normals[i][1], z + normals[i][2])].renderNeighborFaces)
				{
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][0] + x);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][1] + y);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][2] + z);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][0] + x);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][1] + y);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][2] + z);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][0] + x);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][1] + y);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][2] + z);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][0] + x);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][1] + y);
					chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][2] + z);

					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);
					chunk->colors.pfVectorAddfloat(&chunk->colors, 1.0f);

					uint8_t textureID = 0;
					switch (i)
					{
					case 0:
						textureID = voxels[ID].frontFace;
						break;
					case 1:
						textureID = voxels[ID].backFace;
						break;
					case 2:
						textureID = voxels[ID].leftFace;
						break;
					case 3:
						textureID = voxels[ID].rightFace;
						break;
					case 4:
						textureID = voxels[ID].topFace;
						break;
					case 5:
						textureID = voxels[ID].bottomFace;
						break;
					
					default:
						break;
					}

					float uvy = (float)(textureID / TEXTURE_ATLAS_SIZE_IN_BLOCKS);
					float uvx = (float)(textureID - (uvy * TEXTURE_ATLAS_SIZE_IN_BLOCKS));

					float normalized = 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS;

					uvx *= normalized;
					uvy *= normalized;

					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvx + UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvy + UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvx + UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvy + normalized - UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvx + normalized - UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvy + UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvx + normalized - UV_OFFSET);
					chunk->uvs.pfVectorAddfloat(&chunk->uvs, uvy + normalized - UV_OFFSET);

					chunk->indices.pfVectorAddint(&chunk->indices, chunk->indiceIndex);
					chunk->indices.pfVectorAddint(&chunk->indices, chunk->indiceIndex + 1);
					chunk->indices.pfVectorAddint(&chunk->indices, chunk->indiceIndex + 2);
					chunk->indices.pfVectorAddint(&chunk->indices, chunk->indiceIndex + 2);
					chunk->indices.pfVectorAddint(&chunk->indices, chunk->indiceIndex + 1);
					chunk->indices.pfVectorAddint(&chunk->indices, chunk->indiceIndex + 3);
					chunk->indiceIndex += 4;
				}
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
	glBufferData(GL_ARRAY_BUFFER, chunk->vertices.pfVectorTotalfloat(&chunk->vertices) * sizeof(float), (const void *)chunk->vertices.vectorListfloat.items, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->indices.pfVectorTotalint(&chunk->indices) * sizeof(int), (const void *)chunk->indices.vectorListint.items, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, chunk->CBO);
	glBufferData(GL_ARRAY_BUFFER, chunk->colors.pfVectorTotalfloat(&chunk->colors) * sizeof(float), (const void *)chunk->colors.vectorListfloat.items, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, chunk->UVBO);
	glBufferData(GL_ARRAY_BUFFER, chunk->uvs.pfVectorTotalfloat(&chunk->uvs) * sizeof(float), (const void *)chunk->uvs.vectorListfloat.items, GL_STATIC_DRAW);

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

	chunk->renderable = true;
}

void DeleteChunk(struct Chunk *chunk)
{
    glDeleteVertexArrays(1, &chunk->VAO);
	glDeleteBuffers(1, &chunk->VBO);
	glDeleteBuffers(1, &chunk->EBO);
	glDeleteBuffers(1, &chunk->CBO);
	glDeleteBuffers(1, &chunk->UVBO);

	chunk->vertices.pfVectorFreefloat(&chunk->vertices);
	chunk->indices.pfVectorFreeint(&chunk->indices);
	chunk->colors.pfVectorFreefloat(&chunk->colors);
	chunk->uvs.pfVectorFreefloat(&chunk->uvs);
}

void ClearChunk(struct Chunk *chunk)
{
	chunk->indiceIndex = 0;
	chunk->vertices.pfVectorFreefloat(&chunk->vertices);
	chunk->indices.pfVectorFreeint(&chunk->indices);
	chunk->colors.pfVectorFreefloat(&chunk->colors);
	chunk->uvs.pfVectorFreefloat(&chunk->uvs);
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
	glDrawElements(GL_TRIANGLES, chunk->indices.pfVectorTotalint(&chunk->indices), GL_UNSIGNED_INT, 0);
}

void CreateVoxels(struct Chunk *chunk)
{
	for(int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for(int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				//printf("%i", chunk->x);
				chunk->voxels[x][y][z] = GenerateVoxel(chunk->position, x, y, z);
			}
		}
	}

	chunk->populated = true;
}

void CreateVertices(struct Chunk *chunk)
{
	VectorInitfloat(&chunk->vertices);
    VectorInitfloat(&chunk->colors);
    VectorInitfloat(&chunk->uvs);
    VectorInitint(&chunk->indices);

	

	for(int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for(int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				CreateVoxel(chunk, x, y, z, chunk->voxels[x][y][z]);
			}
		}
	}
}

bool IsVoxelInChunk(struct Chunk *chunk, int x, int y, int z)
{
	if (x < 0 || x > CHUNK_SIZE_X - 1 || y < 0 || y > CHUNK_SIZE_Y - 1 || z < 0 || z > CHUNK_SIZE_Z - 1)
		return false;
	else
		return true;
}