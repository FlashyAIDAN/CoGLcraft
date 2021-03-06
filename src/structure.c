#include "structure.h"

vectorvoxelmod GenerateStructure(enum StructureType type, ivec3s position, int minHeight, int maxHeight, uint8_t ID)
{
	VECTOR_INIT(voxelmod, mods);
	switch (type)
	{
	case TREE:
		mods = MakeTree(position, minHeight, maxHeight);
		break;
	
	case CACTUS:
		mods = MakeCactus(position, minHeight, maxHeight);
		break;

	case PLANT:
		mods = MakePlants(position, ID);
		break;
	
	case SMALLHOUSE:
		mods = MakeSmallHouse(position);
		break;

	default:
		break;
	}
	return mods;
}

vectorvoxelmod MakeTree(ivec3s position, int minTrunkHeight, int maxTrunkHeight)
{
    VECTOR_INIT(voxelmod, mods);

	int height = (int)(maxTrunkHeight * Get2DSimplex(position.x, position.z, 250.0f, 3.0f));

	if (height < minTrunkHeight)
		height = minTrunkHeight;


	for(int _x = -2; _x < 3; _x++)
		for(int _y = 0; _y < 5; _y++)
			for(int _z = -2; _z < 3; _z++)
                VectorPushBackvoxelmod(&mods, (struct VoxelMod){(ivec3s){position.x + _x, position.y + (height - ((int)floor(minTrunkHeight / 2.0f))) + _y, position.z + _z}, 6});

	for (int i = 1; i < height; i++)
        VectorPushBackvoxelmod(&mods, (struct VoxelMod){(ivec3s){position.x, position.y + i, position.z}, 5});

	return mods;
}

vectorvoxelmod MakeSmallHouse(ivec3s position)
{
	VECTOR_INIT(voxelmod, mods);

	for(int y = 1; y < 6; y++)
		for(int x = -3; x < 4; x++)
			for(int z = -3; z < 4; z++)
				if((y == 1 || y == 5 || (x == -3 || x == 3) || (z == -3 || z == 3)) && ((y != 2 && y != 3) || (x != 0 || z != 3)))
					VectorPushBackvoxelmod(&mods, (struct VoxelMod){(ivec3s){position.x + x, position.y + y, position.z + z}, 8});

	return mods;
}

vectorvoxelmod MakeCactus(ivec3s position, int minTrunkHeight, int maxTrunkHeight)
{
	VECTOR_INIT(voxelmod, mods);

	int height = (int)(maxTrunkHeight * Get2DSimplex(position.x, position.z, 250.0f, 3.0f));

	if (height < minTrunkHeight)
		height = minTrunkHeight;

	for (int i = 1; i < height; i++)
        VectorPushBackvoxelmod(&mods, (struct VoxelMod){(ivec3s){position.x, position.y + i, position.z}, 17});

	return mods;
}

vectorvoxelmod MakePlants(ivec3s position, uint8_t ID)
{
	VECTOR_INIT(voxelmod, mods);

	VectorPushBackvoxelmod(&mods, (struct VoxelMod){(ivec3s){position.x, position.y + 1, position.z}, ID});

	return mods;
}