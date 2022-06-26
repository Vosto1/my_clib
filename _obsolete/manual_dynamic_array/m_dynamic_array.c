#include "m_dynamic_array.h"

m_dynamicArray m_createEmptyDynamicArray()
{
    m_dynamicArray a;
    a.array = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

dim_t m_initDynamicArray(m_dynamicArray *a, dim_t initSize)
{
    Data *temp;
    temp = (Data *)malloc(sizeof(Data) * initSize);
    if (temp != NULL)
    {
        a->array = temp;
        a->size = initSize;
        m_arrayInitialize(a);
        return initSize;
    }
    else
    {
        a->array = NULL;
        errcset(EMEM_ALLOC);
        return -1;
    }
}

void m_freeArray(m_dynamicArray *a)
{
    if (a->array != NULL)
    {
        free(a->array);
        *a = m_createEmptyDynamicArray();
    }
    else
    {
        errcset(EFREE_NULLPTR);
    }
}

void m_arrayInitialize(m_array *a)
{
    Data data = createPlaceholderData();
    for (int i = 0; i < a->size; i++)
    {
        a->array[i] = data;
    }
    a->used = 0;
}

int m_arrayInsert(m_dynamicArray *a, Data item, int index)
{
    if (a == NULL || a->array == NULL)
    {
        errcset(ENULL_ARG);
        return -1;
    }
    Data placeholder = createPlaceholderData();
    if (index < a->size)
    {
        if (compareData(a->array[index], placeholder) == 0)
        {
            a->array[index] = item;
            a->used += 1;
            return index;
        }
        else
        {
            errcset(EARR_INDEX_OCCUPIED);
            return -1;
        }
    }
    else
    {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return -1;
    }
}

int m_arrayRemoveAt(m_dynamicArray *a, int index)
{
    Data placeholder = createPlaceholderData();
    if (index >= a->size)
    {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return -1;
    }
    else if (compareData(a->array[index], placeholder) == 0)
    {
        return index;
    }
    a->array[index] = createPlaceholderData();
    a->used -= 1;
    return index;
}

int m_arrayCopy(m_dynamicArray *copy, m_dynamicArray *out)
{
    *out = m_createEmptyDynamicArray();
    if (m_initDynamicArray(out, copy->size) != copy->size)
    {
        return -1;
    }
    for (int i = 0; i < copy->size; i++)
    {
        m_arrayInsert(out, copy->array[i], i);
    }
    return out->size;
}

bool m_checkMemoryIncrease(m_dynamicArray *a)
{
    return a->used == a->size;
}

bool m_checkMemoryDecrease(m_dynamicArray *a)
{
    if (a->size == 1)
        return false;
    double ratio = (double)a->used / (double)a->size;
    return ratio <= QUARTER;
}

MEM m_memoryDecrease(m_dynamicArray *a)
{
    // halve allocated space
    if (a->size != 1)
    {
        a->size /= 2;
        Data *temp;
        temp = (Data *)realloc(a->array, sizeof(Data) * a->size);
        if (temp != NULL)
        {
            a->array = temp;
            return MEM_HALVED;
        }
        else
        {
            errcset(EMEM_DREALLOC);
            return ERRMEM_DECREASE;
        }
    }
    return MEM_MS_REACHED;
}

MEM m_memoryIncrease(m_dynamicArray *a)
{
    a->size *= 2;
    Data *temp;
    Data placeholder = createPlaceholderData();
    temp = (Data *)malloc(sizeof(Data) * a->size);
    if (temp != NULL)
    {
        for (int i = 0; i < a->size; i++)
        {
            temp[i] = placeholder;
        }
        for (int i = 0; i < a->size / 2; i++)
        {
            temp[i] = a->array[i];
        }
        free(a->array);
        a->array = temp;
        return MEM_DOUBLED;
    }
    else
    {
        errcset(EMEM_IREALLOC);
        return NMEM_INCREASE;
    }
}