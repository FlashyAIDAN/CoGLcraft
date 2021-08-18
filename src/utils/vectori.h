#ifndef VECTORI_H
#define VECTORI_H

// From: https://aticleworld.com/implement-vector-in-c/

#include <stdio.h>

#define VECTOR_INIT_CAPACITY 1
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTORI_INIT(vec) vectori vec;\
    VectoriInit(&vec)

typedef struct sVectoriList
{
    int *items;
    int capacity;
    int total;
} sVectoriList;

typedef struct sVectori vectori;
struct sVectori
{
    sVectoriList vectorList;

    int (*pfVectorTotal) (vectori *);
    int (*pfVectorResize) (vectori *, int);
    int (*pfVectorAdd) (vectori *, int);
    int (*pfVectorSet) (vectori *, int, int);
    int (*pfVectorDelete) ( vectori *, int);
    int (*pfVectorFree) (vectori *);
    int (*pfVectorGet) (vectori *, int);
};

void VectoriInit(vectori *v);

int VectoriGet(vectori *v, int index);
int VectoriTotal(vectori *v);
int VectoriResize(vectori *v, int capacity);
int VectoriPushBack(vectori *v, int item);
int VectoriSet(vectori *v, int index, int item);
int VectoriDelete(vectori *v, int index);
int VectoriFree(vectori *v);

#endif