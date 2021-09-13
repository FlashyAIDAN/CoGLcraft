#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "world.h"
#include "noise.h"

#include "vendor/ivec3s.h"

vectorvoxelmod GenerateStructure(enum StructureType type, ivec3s position, int minHeight, int maxHeight);
vectorvoxelmod MakeTree(ivec3s position, int minTrunkHeight, int maxTrunkHeight);
vectorvoxelmod MakeCactus(ivec3s position, int minTrunkHeight, int maxTrunkHeight);

#endif