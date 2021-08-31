#ifndef VECTOR_H
#define VECTOR_H

// From: https://aticleworld.com/implement-vector-in-c/

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INIT_CAPACITY 1
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTOR_INIT(vec) vector vec;\
    VectorInit(&vec)
#define VECTORF_INIT(vec) vectorf vec;\
    VectorfInit(&vec)
#define VECTORI_INIT(vec) vectori vec;\
    VectoriInit(&vec)

// Void Vector

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

// Float vector

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

// Int vector

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