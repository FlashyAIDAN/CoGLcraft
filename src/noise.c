#include "noise.h"
#include "chunk.h"

struct osn_context *ctx;

void SimplexInit(int seed)
{
	open_simplex_noise(seed, &ctx);
}

void SimplexFree()
{
	open_simplex_noise_free(ctx);
}

float Get2DPerlin(int seed, float x, float y, float offset, float scale)
{
    return PerlinGet2D(seed, (x + 0.1f) / CHUNK_SIZE_X * scale + offset, (y + 0.1f) / CHUNK_SIZE_Z * scale + offset, 1, 1);
}
float Get2DSimplex(float x, float y, float offset, float scale)
{	
	return (float)open_simplex_noise2(ctx, (x + 0.1f) / CHUNK_SIZE_X * scale + offset, (y + 0.1f) / CHUNK_SIZE_Z * scale + offset);
}

bool Get3DPerlin(int seed, float x, float y, float z, float offset, float scale, float threshold)
{
    float _x = (x + offset + 0.1f) * scale;
	float _y = (y + offset + 0.1f) * scale;
	float _z = (z + offset + 0.1f) * scale;

	float AB = PerlinGet2D(seed, _x, _y, 1, 1);
	float BC = PerlinGet2D(seed, _y, _z, 1, 1);
	float AC = PerlinGet2D(seed, _x, _z, 1, 1);
	float BA = PerlinGet2D(seed, _y, _x, 1, 1);
	float CB = PerlinGet2D(seed, _z, _y, 1, 1);
	float CA = PerlinGet2D(seed, _z, _x, 1, 1);
	if ((AB + BC + AC + BA + CB + CA) / 6.0f > threshold)
		return true;
	else return false;
}