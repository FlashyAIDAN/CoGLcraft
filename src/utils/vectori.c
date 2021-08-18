#include "vectori.h"

// From: https://aticleworld.com/implement-vector-in-c/

void VectoriInit(vectori *v)
{
    v->pfVectorTotal = VectoriTotal;
    v->pfVectorResize = VectoriResize;
    v->pfVectorAdd = VectoriPushBack;
    v->pfVectorSet = VectoriSet;
    v->pfVectorGet = VectoriGet;
    v->pfVectorFree = VectoriFree;
    v->pfVectorDelete = VectoriDelete;

    v->vectorList.capacity = VECTOR_INIT_CAPACITY;
    v->vectorList.total = 0;
    v->vectorList.items = malloc(sizeof(float) * v->vectorList.capacity);
}

int VectoriTotal(vectori *v)
{
    int totalCount = UNDEFINE;
    if(v)
        totalCount = v->vectorList.total;

        return totalCount;
}

int VectoriResize(vectori *v, int capacity)
{
    int  status = UNDEFINE;
    if(v)
    {
        int *items = realloc(v->vectorList.items, sizeof(int) * capacity);
        if (items)
        {
            v->vectorList.items = items;
            v->vectorList.capacity = capacity;
            status = SUCCESS;
        }
    }
    return status;
}

int VectoriPushBack(vectori *v, int item)
{
    int status = UNDEFINE;
    if(v)
    {
        if(v->vectorList.capacity == v->vectorList.total)
        {
            status = VectoriResize(v, v->vectorList.capacity * 2);
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

int VectoriSet(vectori *v, int index, int item)
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

int VectoriGet(vectori *v, int index)
{
    int readData = 0.0f;
    if(v)
    {
        if((index >= 0) && (index < v->vectorList.total))
            readData = v->vectorList.items[index];
    }
    return readData;
}

int VectoriDelete(vectori *v, int index)
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
            VectoriResize(v, v->vectorList.capacity / 2);
        
        status = SUCCESS;
    }
    return status;
}

int VectoriFree(vectori *v)
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