#include "chunk.h"
#include "world.h"

defineFuntionsVector(float, 0);
defineFuntionsVector(int, 0);
defineFuntionsVector(voxelmod, ((struct VoxelMod){(ivec3s){0, 0, 0}, 0}))

struct Voxel voxels[19] =
{
	{"Air", -1, -1, -1, -1, -1, -1, true, true, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Dirt", 242, 242, 242, 242, 242, 242, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Grass", 243, 243, 243, 243, 240, 242, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Stone", 241, 241, 241, 241, 241, 241, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Bedrock", 225, 225, 225, 225, 225, 225, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Log", 228, 228, 228, 228, 229, 229, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Leaf", 196, 196, 196, 196, 196, 196, false, true, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Glass", 193, 193, 193, 193, 193, 193, false, true, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Planks", 244, 244, 244, 244, 244, 244, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Bricks", 247, 247, 247, 247, 247, 247, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Cobblestone", 224, 224, 224, 224, 224, 224, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Gold Ore", 208, 208, 208, 208, 208, 208, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Iron Ore", 209, 209, 209, 209, 209, 209, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Coal Ore", 210, 210, 210, 210, 210, 210, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Diamond Ore", 194, 194, 194, 194, 194, 194, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Redstone Ore", 195, 195, 195, 195, 195, 195, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Sand", 226, 226, 226, 226, 226, 226, false, false, false, false, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Cactus", 182, 182, 182, 182, 183, 183, false, true, true, false, true, {{0.0f, 0.0f, -0.05f}, {0.0f, 0.0f, 0.05f}, {0.05f, 0.0f, 0.0f}, {-0.05f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Rose", 252, 252, 252, 252, 252, 252, false, true, true, true, false, {{0.0f, 0.0f, -0.5f}, {0.0f, 0.0f, 0.5f}, {0.5f, 0.0f, 0.0f}, {-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}}
};

float verticeData[8][3] = 
{
 	{0.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f}
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
	chunk->modified = false;

	VectorInitvoxelmod(&chunk->modifications);
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
	if(!voxels[ID].invisible)
	{
		if(!voxels[ID].cross)
		{
			for(int i = 0; i < 6; i++)
			{
				if(voxels[ID].forceRenderFaces || voxels[GetVoxel(chunk, x + normals[i][0], y + normals[i][1], z + normals[i][2])].renderNeighborFaces)
				{
					vec3s voxelModMeshFace;

					switch (i)
					{
					case 0:
						voxelModMeshFace = voxels[ID].voxelMeshMod.frontFace;
						break;
					case 1:
						voxelModMeshFace = voxels[ID].voxelMeshMod.backFace;
						break;
					case 2:
						voxelModMeshFace = voxels[ID].voxelMeshMod.leftFace;
						break;
					case 3:
						voxelModMeshFace = voxels[ID].voxelMeshMod.rightFace;
						break;
					case 4:
						voxelModMeshFace = voxels[ID].voxelMeshMod.topFace;
						break;
					case 5:
						voxelModMeshFace = voxels[ID].voxelMeshMod.bottomFace;
						break;
					
					default:
						break;
					}

						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][0] + voxelModMeshFace.x + x);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][1] + voxelModMeshFace.y + y);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][2] + voxelModMeshFace.z + z);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][0] + voxelModMeshFace.x + x);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][1] + voxelModMeshFace.y + y);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][2] + voxelModMeshFace.z + z);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][0] + voxelModMeshFace.x + x);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][1] + voxelModMeshFace.y + y);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][2] + voxelModMeshFace.z + z);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][0] + voxelModMeshFace.x + x);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][1] + voxelModMeshFace.y + y);
						chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][2] + voxelModMeshFace.z + z);

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
		else
		{
			for(int i = 0; i < 4; i++)
			{
				vec3s voxelModMeshFace;
				switch (i)
				{
				case 0:
					voxelModMeshFace = voxels[ID].voxelMeshMod.frontFace;
					break;
				case 1:
					voxelModMeshFace = voxels[ID].voxelMeshMod.backFace;
					break;
				case 2:
					voxelModMeshFace = voxels[ID].voxelMeshMod.leftFace;
					break;
				case 3:
					voxelModMeshFace = voxels[ID].voxelMeshMod.rightFace;
					break;
				case 4:
					voxelModMeshFace = voxels[ID].voxelMeshMod.topFace;
					break;
				case 5:
					voxelModMeshFace = voxels[ID].voxelMeshMod.bottomFace;
					break;
				
				default:
					break;
				}

				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][0] + voxelModMeshFace.x + x);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][1] + voxelModMeshFace.y + y);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][0]][2] + voxelModMeshFace.z + z);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][0] + voxelModMeshFace.x + x);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][1] + voxelModMeshFace.y + y);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][1]][2] + voxelModMeshFace.z + z);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][0] + voxelModMeshFace.x + x);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][1] + voxelModMeshFace.y + y);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][2]][2] + voxelModMeshFace.z + z);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][0] + voxelModMeshFace.x + x);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][1] + voxelModMeshFace.y + y);
				chunk->vertices.pfVectorAddfloat(&chunk->vertices, verticeData[indiceData[i][3]][2] + voxelModMeshFace.z + z);

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

	while (VectorTotalvoxelmod(&chunk->modifications) > 0)
	{
		struct VoxelMod voxel = VectorGetvoxelmod(&chunk->modifications, 0);
		VectorDeletevoxelmod(&chunk->modifications, 0);

		chunk->voxels[(int)(voxel.position.x - chunk->position.x)][(int)(voxel.position.y - chunk->position.y)][(int)(voxel.position.z - chunk->position.z)] = voxel.ID;
	}

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