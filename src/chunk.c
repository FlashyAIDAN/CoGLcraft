#include "chunk.h"
#include "world.h"

#define LIGHT_FALLOFF 1
#define LIGHT_RAY_FALLOFF 0.08f

defineFuntionsVector(float, 0);
defineFuntionsVector(int, 0);
defineFuntionsVector(voxelmod, ((struct VoxelMod){(ivec3s){0, 0, 0}, 0}))

struct Voxel voxels[19] =
{
	{"Air", 0.0f, 1.0f, -1, -1, -1, -1, -1, -1, 0, true, true, false, false, false, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Dirt", 5.0f, 0.0f, 242, 242, 242, 242, 242, 242, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Grass", 6.0f, 0.0f, 243, 243, 243, 243, 240, 242, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Stone", 10.0f, 0.0f, 241, 241, 241, 241, 241, 241, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Bedrock", 100.0f, 0.0f, 225, 225, 225, 225, 225, 225, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Log", 10.0f, 0.0f, 228, 228, 228, 228, 229, 229, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Leaf", 1.0f, 0.75f, 196, 196, 196, 196, 196, 196, 0, false, true, false, false, true, true, true, true, true, true, true, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Glass", 1.0f, 1.0f, 193, 193, 193, 193, 193, 193, 0, false, true, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Planks", 10.0f, 0.0f, 244, 244, 244, 244, 244, 244, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Bricks", 25.0f, 0.0f, 247, 247, 247, 247, 247, 247, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Cobblestone", 15.0f, 0.0f, 224, 224, 224, 224, 224, 224, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Gold Ore", 15.0f, 0.0f, 208, 208, 208, 208, 208, 208, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Iron Ore", 20.0f, 0.0f, 209, 209, 209, 209, 209, 209, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Coal Ore", 20.0f, 0.0f, 210, 210, 210, 210, 210, 210, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Diamond Ore", 30.0f, 0.0f, 194, 194, 194, 194, 194, 194, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Redstone Ore", 20.0f, 0.0f, 195, 195, 195, 195, 195, 195, 15, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Sand", 5.0f, 0.0f, 226, 226, 226, 226, 226, 226, 0, false, false, false, false, true, false, false, false, false, false, false, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Cactus", 5.0f, 0.0f, 182, 182, 182, 182, 181, 183, 0, false, true, true, false, true, true, true, true, true, false, false, {{0.0f, 0.0f, -0.05f}, {0.0f, 0.0f, 0.05f}, {0.05f, 0.0f, 0.0f}, {-0.05f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}},
	{"Rose", 1.0f, 0.0f, 252, 252, 252, 252, 252, 252, 0, false, true, true, true, false, true, true, true, true, true, true, {{0.0f, 0.0f, -0.5f}, {0.0f, 0.0f, 0.5f}, {0.5f, 0.0f, 0.0f}, {-0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}}
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
	
    chunk->VAO = 0;
    chunk->VBO = 0;
    chunk->EBO = 0;
    chunk->CBO = 0;
    chunk->UVBO = 0;
	chunk->indiceIndex = 0;

    chunk->position = (vec3s){(float)x * CHUNK_SIZE_X, (float)y * CHUNK_SIZE_Y, (float)z * CHUNK_SIZE_Z};

	chunk->renderable = false;
	chunk->populated = false;
	chunk->modified = false;
	chunk->inVector = false;

	VectorInitvoxelmod(&chunk->modifications);
}

uint8_t GetVoxel(struct Chunk *chunk, int x, int y, int z)
{
	if(!IsVoxelInChunk(chunk, x, y, z))
		return WorldGetVoxel(x + chunk->position.x, y + chunk->position.y, z + chunk->position.z);
	else
		return chunk->voxels[x][y][z];
}

void CreateVoxel(struct Chunk *chunk, struct MeshData *meshData, int x, int y, int z, uint8_t ID)
{
	if(!voxels[ID].invisible)
	{
		if(!voxels[ID].cross)
		{
			for(int i = 0; i < 6; i++)
			{
				uint8_t neighbor = GetVoxel(chunk, x + normals[i][0], y + normals[i][1], z + normals[i][2]);
				if((ID != neighbor && voxels[ID].forceRenderFaces) || (ID != neighbor && voxels[neighbor].renderNeighborFaces) || (i == 0 && ID == neighbor && voxels[ID].renderNeighborsFrontST) || (i == 1 && ID == neighbor && voxels[ID].renderNeighborsBackST) || (i == 2 && ID == neighbor && voxels[ID].renderNeighborsLeftST) | (i == 3 && ID == neighbor && voxels[ID].renderNeighborsRightST) || (i == 4 && ID == neighbor && voxels[ID].renderNeighborsTopST) || (i == 5 && ID == neighbor && voxels[ID].renderNeighborsBottomST))
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

					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][0]][0] + voxelModMeshFace.x + x);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][0]][1] + voxelModMeshFace.y + y);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][0]][2] + voxelModMeshFace.z + z);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][1]][0] + voxelModMeshFace.x + x);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][1]][1] + voxelModMeshFace.y + y);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][1]][2] + voxelModMeshFace.z + z);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][2]][0] + voxelModMeshFace.x + x);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][2]][1] + voxelModMeshFace.y + y);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][2]][2] + voxelModMeshFace.z + z);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][3]][0] + voxelModMeshFace.x + x);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][3]][1] + voxelModMeshFace.y + y);
					VectorPushBackfloat(&meshData->vertices, verticeData[indiceData[i][3]][2] + voxelModMeshFace.z + z);

					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
					VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);

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
					VectorPushBackfloat(&meshData->uvs, uvx + UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvy + UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvx + UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvy + normalized - UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvx + normalized - UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvy + UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvx + normalized - UV_OFFSET);
					VectorPushBackfloat(&meshData->uvs, uvy + normalized - UV_OFFSET);

					VectorPushBackint(&meshData->indices, chunk->indiceIndex);
					VectorPushBackint(&meshData->indices, chunk->indiceIndex + 1);
					VectorPushBackint(&meshData->indices, chunk->indiceIndex + 2);
					VectorPushBackint(&meshData->indices, chunk->indiceIndex + 2);
					VectorPushBackint(&meshData->indices, chunk->indiceIndex + 1);
					VectorPushBackint(&meshData->indices, chunk->indiceIndex + 3);
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
				
				default:
					break;
				}
				vec3s v1 = glms_vec3_rotate((vec3s){verticeData[indiceData[i][0]][0] + voxelModMeshFace.x, verticeData[indiceData[i][0]][1] + voxelModMeshFace.y, verticeData[indiceData[i][0]][2] + voxelModMeshFace.z}, glm_rad(45.0f), (vec3s){0.0f, 1.0f, 0.0f});
				vec3s v2 = glms_vec3_rotate((vec3s){verticeData[indiceData[i][1]][0] + voxelModMeshFace.x, verticeData[indiceData[i][1]][1] + voxelModMeshFace.y, verticeData[indiceData[i][1]][2] + voxelModMeshFace.z}, glm_rad(45.0f), (vec3s){0.0f, 1.0f, 0.0f});
				vec3s v3 = glms_vec3_rotate((vec3s){verticeData[indiceData[i][2]][0] + voxelModMeshFace.x, verticeData[indiceData[i][2]][1] + voxelModMeshFace.y, verticeData[indiceData[i][2]][2] + voxelModMeshFace.z}, glm_rad(45.0f), (vec3s){0.0f, 1.0f, 0.0f});
				vec3s v4 = glms_vec3_rotate((vec3s){verticeData[indiceData[i][3]][0] + voxelModMeshFace.x, verticeData[indiceData[i][3]][1] + voxelModMeshFace.y, verticeData[indiceData[i][3]][2] + voxelModMeshFace.z}, glm_rad(45.0f), (vec3s){0.0f, 1.0f, 0.0f});

				VectorPushBackfloat(&meshData->vertices, v1.x + x - 0.25f);
				VectorPushBackfloat(&meshData->vertices, v1.y + y);
				VectorPushBackfloat(&meshData->vertices, v1.z + z + 0.5f);
				VectorPushBackfloat(&meshData->vertices, v2.x + x - 0.25f);
				VectorPushBackfloat(&meshData->vertices, v2.y + y);
				VectorPushBackfloat(&meshData->vertices, v2.z + z + 0.5f);
				VectorPushBackfloat(&meshData->vertices, v3.x + x - 0.25f);
				VectorPushBackfloat(&meshData->vertices, v3.y + y);
				VectorPushBackfloat(&meshData->vertices, v3.z + z + 0.5f);
				VectorPushBackfloat(&meshData->vertices, v4.x + x - 0.25f);
				VectorPushBackfloat(&meshData->vertices, v4.y + y);
				VectorPushBackfloat(&meshData->vertices, v4.z + z + 0.5f);

				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);
				VectorPushBackfloat(&meshData->colors, (float)chunk->lights[x][y][z] / 15.0f);

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
				
				default:
					break;
				}

				float uvy = (float)(textureID / TEXTURE_ATLAS_SIZE_IN_BLOCKS);
				float uvx = (float)(textureID - (uvy * TEXTURE_ATLAS_SIZE_IN_BLOCKS));
				float normalized = 1.0f / TEXTURE_ATLAS_SIZE_IN_BLOCKS;
				uvx *= normalized;
				uvy *= normalized;
				VectorPushBackfloat(&meshData->uvs, uvx + UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvy + UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvx + UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvy + normalized - UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvx + normalized - UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvy + UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvx + normalized - UV_OFFSET);
				VectorPushBackfloat(&meshData->uvs, uvy + normalized - UV_OFFSET);

				VectorPushBackint(&meshData->indices, chunk->indiceIndex);
				VectorPushBackint(&meshData->indices, chunk->indiceIndex + 1);
				VectorPushBackint(&meshData->indices, chunk->indiceIndex + 2);
				VectorPushBackint(&meshData->indices, chunk->indiceIndex + 2);
				VectorPushBackint(&meshData->indices, chunk->indiceIndex + 1);
				VectorPushBackint(&meshData->indices, chunk->indiceIndex + 3);
				chunk->indiceIndex += 4;
			}
		}
	}
}

void CreateChunkBufferData(struct Chunk *chunk, struct MeshData *meshData)
{
    glGenVertexArrays(1, &chunk->VAO);
	glGenBuffers(1, &chunk->VBO);
	glGenBuffers(1, &chunk->EBO);
	glGenBuffers(1, &chunk->CBO);
	glGenBuffers(1, &chunk->UVBO);

	glBindVertexArray(chunk->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
	glBufferData(GL_ARRAY_BUFFER, VectorTotalfloat(&meshData->vertices) * sizeof(float), (const void *)meshData->vertices.vectorListfloat.items, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, VectorTotalint(&meshData->indices) * sizeof(int), (const void *)meshData->indices.vectorListint.items, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, chunk->CBO);
	glBufferData(GL_ARRAY_BUFFER, VectorTotalfloat(&meshData->colors) * sizeof(float), (const void *)meshData->colors.vectorListfloat.items, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, chunk->UVBO);
	glBufferData(GL_ARRAY_BUFFER, VectorTotalfloat(&meshData->uvs) * sizeof(float), (const void *)meshData->uvs.vectorListfloat.items, GL_STATIC_DRAW);

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
}

void ClearChunk(struct Chunk *chunk)
{
	chunk->indiceIndex = 0;
	glDeleteVertexArrays(1, &chunk->VAO);
	glDeleteBuffers(1, &chunk->VBO);
	glDeleteBuffers(1, &chunk->EBO);
	glDeleteBuffers(1, &chunk->CBO);
	glDeleteBuffers(1, &chunk->UVBO);
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
	glDrawElements(GL_TRIANGLES, chunk->indiceIndex, GL_UNSIGNED_INT, 0);
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

void CreateVertices(struct Chunk *chunk, bool shade)
{
	struct MeshData meshData;
	VectorInitfloat(&meshData.vertices);
    VectorInitfloat(&meshData.colors);
    VectorInitfloat(&meshData.uvs);
    VectorInitint(&meshData.indices);

	while (VectorTotalvoxelmod(&chunk->modifications) > 0)
	{
		struct VoxelMod voxel = VectorGetvoxelmod(&chunk->modifications, 0);
		VectorDeletevoxelmod(&chunk->modifications, 0);

		chunk->voxels[(int)(voxel.position.x - chunk->position.x)][(int)(voxel.position.y - chunk->position.y)][(int)(voxel.position.z - chunk->position.z)] = voxel.ID;
	}

	if(shade)
		CalculateLight(chunk);

	for(int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for(int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				CreateVoxel(chunk, &meshData, x, y, z, chunk->voxels[x][y][z]);
			}
		}
	}

	CreateChunkBufferData(chunk, &meshData);

	chunk->indiceIndex = VectorTotalint(&meshData.indices); // NOTE(Aidan): THIS IS JUST A SMALL OPTIMIZATION THAT MIGHT NOT WORK OR IT WILL CAUSE ERROES BUT WHAT IS SUPPOSED TO HAPPEN IS THAT SSINCE WE DONT NEED INDICEINDEX ANYMORE WE CAN NOW USE IT AS A COUNT OF ALL THE INDICIES!!!

	VectorFreefloat(&meshData.vertices);
	VectorFreefloat(&meshData.colors);
	VectorFreefloat(&meshData.uvs);
	VectorFreeint(&meshData.indices);
}

void CalculateLight(struct Chunk *chunk)
{
	VECTOR_INIT(ivec3s, litVoxels);
	VECTOR_INIT(ivec2s, modifedMesh);

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{
			float lightRay = 0.0f;

			for (int y = CHUNK_SIZE_Y - 1; y >= 0; y--)
			{
				uint8_t thisVoxel = chunk->voxels[x][y][z];

				if (thisVoxel > 0 && voxels[thisVoxel].transparency < lightRay && !voxels[thisVoxel].cross)
					lightRay = voxels[thisVoxel].transparency;

				lightRay += (float)voxels[thisVoxel].lightLevel / 15.0f;
				if(lightRay > 1.0f)
					lightRay = 1.0f;
				
				chunk->lights[x][y][z] = (uint8_t)(lightRay * 100.0f / 6.667f);

				if (lightRay > LIGHT_RAY_FALLOFF)
					VectorPushBackivec3s(&litVoxels, (ivec3s){x, y, z});
			}
		}
	}
	int d = 0;
	while (VectorTotalivec3s(&litVoxels) > d)
	{
		ivec3s v = VectorGetivec3s(&litVoxels, d);
		d++;
		for (int p = 0; p < 6; p++)
		{
			ivec3s neighbor = (ivec3s){v.x + normals[p][0], v.y + normals[p][1], v.z + normals[p][2]};

			if (IsVoxelInChunk(chunk, neighbor.x, neighbor.y, neighbor.z))
			{
				if (chunk->lights[neighbor.x][neighbor.y][neighbor.z] < chunk->lights[v.x][v.y][v.z] - LIGHT_FALLOFF)
				{
					chunk->lights[neighbor.x][neighbor.y][neighbor.z] = chunk->lights[v.x][v.y][v.z] - LIGHT_FALLOFF;

					if (chunk->lights[neighbor.x][neighbor.y][neighbor.z] > LIGHT_FALLOFF)
						VectorPushBackivec3s(&litVoxels, neighbor);
				}
			}
			else if(IsChunkInWorld(GetCurrentChunkCoordinates((vec2s){chunk->position.x + neighbor.x, chunk->position.z + neighbor.z})) && GetChunk(GetCurrentChunkCoordinates((vec2s){chunk->position.x + neighbor.x, chunk->position.z + neighbor.z}))->renderable) // FIX THIS
			{
				struct Chunk *currentChunk = GetChunk(GetCurrentChunkCoordinates((vec2s){chunk->position.x + neighbor.x, chunk->position.z + neighbor.z}));
				currentChunk->lights[abs((int)currentChunk->position.x - (int)chunk->position.x - neighbor.x)][neighbor.y][abs((int)currentChunk->position.z - (int)chunk->position.z - neighbor.z)] = 1;

				if (currentChunk->lights[abs((int)currentChunk->position.x - (int)chunk->position.x - neighbor.x)][neighbor.y][abs((int)currentChunk->position.z - (int)chunk->position.z - neighbor.z)] > LIGHT_FALLOFF)
					VectorPushBackivec3s(&litVoxels, neighbor);
				if(!currentChunk->modified)
				{
					VectorPushBackivec2s(&modifedMesh, GetCurrentChunkCoordinates((vec2s){chunk->position.x + neighbor.x, chunk->position.z + neighbor.z}));
					currentChunk->modified = true;
				}
			}
		}
	}

	for(int mc = 0; mc < VectorTotalivec2s(&modifedMesh); mc++)
	{
		struct Chunk *currentChunk = GetChunk(VectorGetivec2s(&modifedMesh, mc));
		ClearChunk(currentChunk);
		CreateVertices(currentChunk, false);
		currentChunk->modified = false;
	}

	VectorFreeivec3s(&litVoxels);
}

bool IsVoxelInChunk(struct Chunk *chunk, int x, int y, int z)
{
	if (x < 0 || x > CHUNK_SIZE_X - 1 || y < 0 || y > CHUNK_SIZE_Y - 1 || z < 0 || z > CHUNK_SIZE_Z - 1)
		return false;
	else
		return true;
}