#include "dynamic_array.h"

size_t elementcount(array *a)
{
    return a->used;
}

size_t arraysize(array *a)
{
    return a->size;
}

dynamicArray createEmptyDynamicArray()
{
    dynamicArray a;
    a.array = NULL;
    a.compare = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

size_t initDynamicArray(dynamicArray *a, size_t initSize, int (*compare)(cvoidp x, cvoidp y))
{
    a->size = 0;
    a->used = 0;
    voidp *temp;
    temp = (voidp *)malloc(sizeof(voidp) * initSize);
    if (temp != NULL)
    {
        a->array = temp;
        a->size = initSize;
        a->compare = compare;
        return initSize;
    }
    else
    {
        *a = createEmptyDynamicArray();
        errcset(EMEM_ALLOC);
        return -1;
    }
}

void freeArray(dynamicArray *a)
{
    if (!a_is_null(a))
    {
        if (!a_is_empty(a))
            arrayClear(a);
        free(a->array);
        *a = createEmptyDynamicArray();
    }
    else
    {
        errcset(EFREE_NULLPTR);
    }
}

size_t arrayInsert(dynamicArray *a, voidp item)
{
    if (a == NULL || a->array == NULL)
    {
        errcset(ENULL_ARG);
        return -1;
    }
    // memory increase
    if (a->used == a->size)
    { // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        voidp *temp;
        temp = (voidp *)realloc(a->array, sizeof(voidp) * a->size);
        if (temp != NULL)
        {
            a->array = temp;
        }
        else
        {
            errcset(EMEM_IREALLOC);
            return -1;
        }
    }
    a->used += 1;
    a->array[a->used - 1] = item;
    return a->used;
}

voidp arrayRemoveLast(dynamicArray *a)
{
    if (a_is_empty(a))
    {
        errcset(EARR_EMPTY);
        return NULL;
    }
    a->used -= 1;
    MEM m = memoryDecrease(a);
    if (m != NMEM_DECREASE)
    {
        voidp data = a->array[a->used];
        return data;
    }
    else
    {
        a->used += 1; // rollback
        return NULL;
    }
}

voidp arrayRemoveItem(dynamicArray *a, voidp item)
{
    for (int i = 0; i < a->used; i++)
    {
        if ((*a->compare)(item, a->array[i]) == 0)
        { // compare with user defined function
            return arrayRemoveAt(a, i);
        }
    }
    errcset(EARRDATA_DOESNT_EXIST);
    return NULL;
}

voidp arrayRemoveAt(dynamicArray *a, int index)
{
    if (index > a->used)
    {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    voidp *data = a->array[index];
    for (int i = index; i < a->used; i++)
    {
        a->array[i] = a->array[i + 1];
    }
    a->used -= 1;
    MEM m = memoryDecrease(a);
    if (m != NMEM_DECREASE)
    {
        return data;
    }
    else
    {
        a->used += 1;             // rollback
        a->array[a->used] = data; // rollback
        return NULL;
    }
}

size_t convert(dynamicArray *a, voidp b[], size_t bsize, int (*compare)(cvoidp x, cvoidp y))
{
    *a = createEmptyDynamicArray();
    if (initDynamicArray(a, bsize, compare) != bsize)
    {
        return -1;
    }
    for (int i = 0; i < bsize; i++)
    {
        arrayInsert(a, b[i]);
    }
    return bsize;
}

size_t arrayUnion(dynamicArray *a, dynamicArray *b)
{
    if (a == NULL || b == NULL)
    {
        errcset(ENULL_ARG);
        return -1;
    }
    for (int i = 0; i < b->used; i++)
    {
        arrayInsert(a, b->array[i]);
    }
    // free only array not the elements (freeArray frees all items in the array as well)
    free(b->array);
    b->size = 0;
    b->used = 0;
    b->array = NULL;
    return a->size;
}

size_t arrayClear(array *a)
{
    if (a_is_empty(a))
    {
        errcset(EARR_EMPTY);
        return -1;
    }
    else if (a_is_null(a))
    {
        errcset(ENULL_ARG);
        return -1;
    }
    int amount = a->used;
    voidp d;
    for (int i = 0; i < amount; i++)
    {
        d = arrayRemoveLast(a);
        free(d);
    }
    return amount;
}

bool a_is_null(array *a)
{
    return a->array == NULL;
}

bool a_is_empty(array *a)
{
    return a->used == 0;
}

// linear search
int find(array *a, voidp item)
{
    for (int i = 0; i < elementcount(a); i++)
    {
        if ((*a->compare)(a->array[i], item))
        {
            return i;
        }
    }
    return -1;
}

bool arrayExists(array *a, voidp item)
{
    if (find(a, item) != -1)
    {
        return true;
    }
    else
        return false;
}

// memory check and increase is done in arrayInsert function
static MEM memoryDecrease(dynamicArray *a)
{
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if (ratio <= QUARTER && arraysize(a) != 1)
    {
        a->size /= 2;
        voidp *temp;
        temp = (voidp *)realloc(a->array, sizeof(voidp) * a->size);
        if (temp != NULL)
        {
            a->array = temp;
            return MEM_HALVED;
        }
        else
        {
            errcset(EMEM_DREALLOC);
            return NMEM_DECREASE;
        }
    }
    return MEM_MS_REACHED_NO_NEED;
}
