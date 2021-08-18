#ifndef VECTORF_H
#define VECTORF_H

// From: https://aticleworld.com/implement-vector-in-c/

#include <stdio.h>

#define VECTOR_INIT_CAPACITY 1
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTORF_INIT(vec) vectorf vec;\
    VectorfInit(&vec)

typedef struct sVectorfList
{
    float *items;
    int capacity;
    int total;
} sVectorfList;

typedef struct sVectorf vectorf;
struct sVectorf
{
    sVectorfList vectorList;

    int (*pfVectorTotal) (vectorf *);
    int (*pfVectorResize) (vectorf *, int);
    int (*pfVectorAdd) (vectorf *, float);
    int (*pfVectorSet) (vectorf *, int, float);
    int (*pfVectorDelete) ( vectorf *, int);
    int (*pfVectorFree) (vectorf *);

    float (*pfVectorGet) (vectorf *, int);
};

float VectorfGet(vectorf *v, int index);
void VectorfInit(vectorf *v);

int VectorfTotal(vectorf *v);
int VectorfResize(vectorf *v, int capacity);
int VectorfPushBack(vectorf *v, float item);
int VectorfSet(vectorf *v, int index, float item);
int VectorfDelete(vectorf *v, int index);
int VectorfFree(vectorf *v);

#endif