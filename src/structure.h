#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "world.h"
#include "noise.h"

#include "vendor/ivec3s.h"

vectorvoxelmod MakeTree(ivec3s position, int minTrunkHeight, int maxTrunkHeight);

#endif