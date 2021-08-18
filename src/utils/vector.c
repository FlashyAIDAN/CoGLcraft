#include "vector.h"

// From: https://aticleworld.com/implement-vector-in-c/

void VectorInit(vector *v)
{
    v->pfVectorTotal = VectorTotal;
    v->pfVectorResize = VectorResize;
    v->pfVectorAdd = VectorPushBack;
    v->pfVectorSet = VectorSet;
    v->pfVectorGet = VectorGet;
    v->pfVectorFree = VectorFree;
    v->pfVectorDelete = VectorDelete;

    v->vectorList.capacity = VECTOR_INIT_CAPACITY;
    v->vectorList.total = 0;
    v->vectorList.items = malloc(sizeof(void *) * v->vectorList.capacity);
}

int VectorTotal(vector *v)
{
    int totalCount = UNDEFINE;
    if(v)
        totalCount = v->vectorList.total;

        return totalCount;
}

int VectorResize(vector *v, int capacity)
{
    int  status = UNDEFINE;
    if(v)
    {
        void **items = realloc(v->vectorList.items, sizeof(void *) * capacity);
        if (items)
        {
            v->vectorList.items = items;
            v->vectorList.capacity = capacity;
            status = SUCCESS;
        }
    }
    return status;
}

int VectorPushBack(vector *v, void *item)
{
    int status = UNDEFINE;
    if(v)
    {
        if(v->vectorList.capacity == v->vectorList.total)
        {
            status = VectorResize(v, v->vectorList.capacity * 2);
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

int VectorSet(vector *v, int index, void *item)
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

void *VectorGet(vector *v, int index)
{
    void *readData = NULL;
    if(v)
    {
        if((index >= 0) && (index < v->vectorList.total))
            readData = v->vectorList.items[index];
    }
    return readData;
}

int VectorDelete(vector *v, int index)
{
    int status = UNDEFINE;
    int i = 0;
    if(v)
    {
        if((index < 0) || (index >= v->vectorList.total))
            return status;

        v->vectorList.items[index] = NULL;

        for(i = index; (i < v->vectorList.total - 1); ++i)
        {
            v->vectorList.items[i] = v->vectorList.items[i + 1];
            v->vectorList.items[i + 1] = NULL;
        }

        v->vectorList.total--;

        if((v->vectorList.total > 0) && ((v->vectorList.total) == (v->vectorList.capacity / 4)))
            VectorResize(v, v->vectorList.capacity / 2);
        
        status = SUCCESS;
    }
    return status;
}

int VectorFree(vector *v)
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