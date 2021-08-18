#include "vectorf.h"

// From: https://aticleworld.com/implement-vector-in-c/

void VectorfInit(vectorf *v)
{
    v->pfVectorTotal = VectorfTotal;
    v->pfVectorResize = VectorfResize;
    v->pfVectorAdd = VectorfPushBack;
    v->pfVectorSet = VectorfSet;
    v->pfVectorGet = VectorfGet;
    v->pfVectorFree = VectorfFree;
    v->pfVectorDelete = VectorfDelete;

    v->vectorList.capacity = VECTOR_INIT_CAPACITY;
    v->vectorList.total = 0;
    v->vectorList.items = malloc(sizeof(float) * v->vectorList.capacity);
}

int VectorfTotal(vectorf *v)
{
    int totalCount = UNDEFINE;
    if(v)
        totalCount = v->vectorList.total;

        return totalCount;
}

int VectorfResize(vectorf *v, int capacity)
{
    int  status = UNDEFINE;
    if(v)
    {
        float *items = realloc(v->vectorList.items, sizeof(float) * capacity);
        if (items)
        {
            v->vectorList.items = items;
            v->vectorList.capacity = capacity;
            status = SUCCESS;
        }
    }
    return status;
}

int VectorfPushBack(vectorf *v, float item)
{
    int status = UNDEFINE;
    if(v)
    {
        if(v->vectorList.capacity == v->vectorList.total)
        {
            status = VectorfResize(v, v->vectorList.capacity * 2);
            if(status != UNDEFINE)
                v->vectorList.items[v->vectorList.total++] = item;
        }
        else
        {
            v->vectorList.items[v->vectorList.total++] = item;
            status = SUCCESS;
        }
    }
    return status;
}

int VectorfSet(vectorf *v, int index, float item)
{
    int status = UNDEFINE;
    if(v)
    {
        if((index >= 0) && (index < v->vectorList.total))
        {
            v->vectorList.items[index] = item;
            status = SUCCESS;
        }
    }
    return status;
}

float VectorfGet(vectorf *v, int index)
{
    float readData = 0.0f;
    if(v)
    {
        if((index >= 0) && (index < v->vectorList.total))
            readData = v->vectorList.items[index];
    }
    return readData;
}

int VectorfDelete(vectorf *v, int index)
{
    int status = UNDEFINE;
    int i = 0;
    if(v)
    {
        if((index < 0) || (index >= v->vectorList.total))
            return status;

        v->vectorList.items[index] = 0.0f;

        for(i = index; (i < v->vectorList.total - 1); ++i)
        {
            v->vectorList.items[i] = v->vectorList.items[i + 1];
            v->vectorList.items[i + 1] = 0.0f;
        }

        v->vectorList.total--;

        if((v->vectorList.total > 0) && ((v->vectorList.total) == (v->vectorList.capacity / 4)))
            VectorfResize(v, v->vectorList.capacity / 2);
        
        status = SUCCESS;
    }
    return status;
}

int VectorfFree(vectorf *v)
{
    int status = UNDEFINE;
    if(v)
    {
        free(v->vectorList.items);
        v->vectorList.items = NULL;
        status = SUCCESS;
    }
    return status;
}