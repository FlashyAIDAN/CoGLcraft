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
float Get2DSimplex(float x, float y, float offset, float scale)
{	
	return (float)open_simplex_noise2(ctx, (x + 0.1f) / CHUNK_SIZE_X * scale + offset, (y + 0.1f) / CHUNK_SIZE_Z * scale + offset);
}

float Get3DSimplex(float x, float y, float z, float offset, float scale)
{
	return (float)open_simplex_noise3(ctx, (x + 0.1f) * scale + offset, (y + 0.1f) * scale + offset, (z + 0.1f) * scale + offset);
}