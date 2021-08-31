#ifndef WORLD_H
#define WORLD_H

#include "utils/texture.h"
#include "utils/shader.h"

#include "chunk.h"

#define NUMBER_OF_CHUNKS_X 16
#define NUMBER_OF_CHUNKS_Z 16

struct Chunk chunks[NUMBER_OF_CHUNKS_X][NUMBER_OF_CHUNKS_Z];

void WorldStart();
void WorldRender(struct Texture2D *texture, struct Shader *shader);
void WorldDelete();

uint8_t WorldGetVoxel(int x, int y, int z);

#endif