#include "structure.h"

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