#ifndef VECTOR_H
#define VECTOR_H

// From: https://aticleworld.com/implement-vector-in-c/
// From: https://stackoverflow.com/questions/10950828/simulation-of-templates-in-c-for-a-queue-data-type (Look here for help on the limitations section)

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INIT_CAPACITY 1
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTOR_INIT(type, vec) vector##type vec;\
    VectorInit##type(&vec)

#define defineVector(type) \
\
typedef struct sVectorList##type \
{ \
    type *items; \
    int capacity; \
    int total; \
} sVectorList##type; \
\
typedef struct sVector##type vector##type; \
struct sVector##type \
{ \
    sVectorList##type vectorList##type; \
\
    int (*pfVectorTotal##type) (vector##type *); \
    int (*pfVectorResize##type) (vector##type *, int); \
    int (*pfVectorAdd##type) (vector##type *, type); \
    int (*pfVectorSet##type) (vector##type *, int, type); \
    int (*pfVectorDelete##type) ( vector##type *, int); \
    int (*pfVectorFree##type) (vector##type *); \
\
    type (*pfVectorGet##type) (vector##type *, int); \
}; \
\
type VectorGet##type(vector##type *v, int index); \
void VectorInit##type(vector##type *v); \
\
int VectorTotal##type(vector##type *v); \
int VectorResize##type(vector##type *v, int capacity); \
int VectorPushBack##type(vector##type *v, type item); \
int VectorSet##type(vector##type *v, int index, type item); \
int VectorDelete##type(vector##type *v, int index); \
int VectorFree##type(vector##type *v);

#define defineFuntionsVector(type, nullInitializer) \
void VectorInit##type(vector##type *v)\
{ \
    v->pfVectorTotal##type = VectorTotal##type; \
    v->pfVectorResize##type = VectorResize##type; \
    v->pfVectorAdd##type = VectorPushBack##type; \
    v->pfVectorSet##type = VectorSet##type; \
    v->pfVectorGet##type = VectorGet##type; \
    v->pfVectorFree##type = VectorFree##type; \
    v->pfVectorDelete##type = VectorDelete##type; \
\
    v->vectorList##type.capacity = VECTOR_INIT_CAPACITY; \
    v->vectorList##type.total = 0; \
    v->vectorList##type.items = malloc(sizeof(type) * v->vectorList##type.capacity); \
} \
\
int VectorTotal##type(vector##type *v) \
{ \
    int totalCount = UNDEFINE; \
    if(v) \
        totalCount = v->vectorList##type.total; \
\
        return totalCount; \
} \
\
int VectorResize##type(vector##type *v, int capacity) \
{ \
    int  status = UNDEFINE; \
    if(v) \
    { \
        type *items = realloc(v->vectorList##type.items, sizeof(type) * capacity); \
        if (items) \
        { \
            v->vectorList##type.items = items; \
            v->vectorList##type.capacity = capacity; \
            status = SUCCESS; \
        } \
    } \
    return status; \
} \
\
int VectorPushBack##type(vector##type *v, type item) \
{ \
    int status = UNDEFINE; \
    if(v) \
    { \
        if(v->vectorList##type.capacity == v->vectorList##type.total) \
        { \
            status = VectorResize##type(v, v->vectorList##type.capacity * 2); \
            if(status != UNDEFINE) \
                v->vectorList##type.items[v->vectorList##type.total++] = item; \
        } \
        else \
        { \
            v->vectorList##type.items[v->vectorList##type.total++] = item; \
            status = SUCCESS; \
        } \
    } \
    return status; \
} \
\
int VectorSet##type(vector##type *v, int index, type item) \
{ \
    int status = UNDEFINE; \
    if(v) \
    { \
        if((index >= 0) && (index < v->vectorList##type.total)) \
        { \
            v->vectorList##type.items[index] = item; \
            status = SUCCESS; \
        } \
    } \
    return status; \
} \
\
type VectorGet##type(vector##type *v, int index) \
{ \
    type readData = nullInitializer; \
    if(v) \
    { \
        if((index >= 0) && (index < v->vectorList##type.total)) \
            readData = v->vectorList##type.items[index]; \
    } \
    return readData; \
} \
\
int VectorDelete##type(vector##type *v, int index) \
{ \
    int status = UNDEFINE; \
    int i = 0; \
    if(v) \
    { \
        if((index < 0) || (index >= v->vectorList##type.total)) \
            return status; \
\
        v->vectorList##type.items[index] = nullInitializer; \
\
        for(i = index; (i < v->vectorList##type.total - 1); ++i) \
        { \
            v->vectorList##type.items[i] = v->vectorList##type.items[i + 1]; \
            v->vectorList##type.items[i + 1] = nullInitializer; \
        } \
\
        v->vectorList##type.total--; \
\
        if((v->vectorList##type.total > 0) && ((v->vectorList##type.total) == (v->vectorList##type.capacity / 4))) \
            VectorResize##type(v, v->vectorList##type.capacity / 2); \
\
        status = SUCCESS; \
    } \
    return status; \
} \
\
int VectorFree##type(vector##type *v) \
{ \
    int status = UNDEFINE; \
    if(v) \
    { \
        free(v->vectorList##type.items); \
        v->vectorList##type.items = NULL; \
        status = SUCCESS; \
    } \
    return status; \
}

#endif