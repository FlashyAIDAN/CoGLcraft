#ifndef VECTOR_H
#define VECTOR_H

// From: https://aticleworld.com/implement-vector-in-c/

#include <stdio.h>

#define VECTOR_INIT_CAPACITY 1
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTOR_INIT(vec) vector vec;\
    VectorInit(&vec)

typedef struct sVectorList
{
    void **items;
    int capacity;
    int total;
} sVectorList;

typedef struct sVector vector;
struct sVector
{
    sVectorList vectorList;

    int (*pfVectorTotal) (vector *);
    int (*pfVectorResize) (vector *, int);
    int (*pfVectorAdd) (vector *, void *);
    int (*pfVectorSet) (vector *, int, void *);
    int (*pfVectorDelete) ( vector *, int);
    int (*pfVectorFree) (vector *);

    void *(*pfVectorGet) (vector *, int);
};

void *VectorGet(vector *v, int index);
void VectorInit(vector *v);

int VectorTotal(vector *v);
int VectorResize(vector *v, int capacity);
int VectorPushBack(vector *v, void *item);
int VectorSet(vector *v, int index, void *item);
int VectorDelete(vector *v, int index);
int VectorFree(vector *v);

#endif