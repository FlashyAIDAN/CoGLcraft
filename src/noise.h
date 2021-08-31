#ifndef NOISE_H
#define NOISE_H

#include "stdbool.h"
#include "utils/perlin.h"
#include "vendor/open-simplex-noise.h"

void SimplexInit(int seed);
void SimplexFree();

// Perlin Doesnt wok and is slower so will prolly remove it in the future
float Get2DPerlin(int seed, float x, float y, float offset, float scale);
float Get2DSimplex(float x, float y, float offset, float scale);

bool Get3DPerlin(int seed, float x, float y, float z, float offset, float scale, float threshold);

#endif