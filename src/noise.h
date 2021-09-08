#ifndef NOISE_H
#define NOISE_H

#include "vendor/open-simplex-noise.h"

void SimplexInit(int seed);
void SimplexFree();

float Get2DSimplex(float x, float y, float offset, float scale);
float Get3DSimplex(float x, float y, float z, float offset, float scale);

#endif