#include "darray.h"

#define QUARTER 0.25

typedef enum
{                               // decrease memory results
    MEM_HALVED = 0,             // memory halved
    MEM_MS_REACHED_NO_NEED = 1, // MEMory Min Size REACHED or no need
    ERRMEM_DECREASE = 2,        // No MEMory DECREASE (error, check errc global)
} MEM;

/**
 * checks if the memory should be halved and does so if check is positive
 *
 * @param a to decrease memory for
 * @return result of memory reduction
 */
static MEM memory_decrease(darray *a);

uint da_count(darray *a)
{
    return a->used;
}

uint da_size(darray *a)
{
    return a->size;
}

darray da_create_empty()
{
    darray a;
    a.array = NULL;
    a.compare = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

int da_init(darray *a, uint initSize, int (*compare)(const void* x, const void* y), void (*freeObject)(void*))
{
    a->size = 0;
    a->used = 0;
    void* *temp;
    temp = (void* *)malloc(sizeof(void*) * initSize);
    if (temp != NULL)
    {
        a->array = temp;
        a->size = initSize;
        a->compare = compare;
        a->freeObject = freeObject;
        return initSize;
    }
    else
    {
        *a = da_create_empty();
        return ERRMEM;
    }
}

bool da_destroy(darray *a)
{
    if (!da_is_null(a))
    {
        free(a->array);
        *a = da_create_empty();
        return true;
    }
    return false;
}

bool da_free(darray *a)
{
    if (!da_is_null(a))
    {
        if (!da_is_empty(a))
        {
            da_clear(a);
            da_destroy(a);
        }
        else
            da_destroy(a);
        return true;
    }
    return false;
}

int da_clear(darray *a)
{
    if (da_is_empty(a))
    {
        return 1;  // its empty; job already done
    }
    else if (da_is_null(a))
    {
        return NULLARG;
    }
    int amount = a->used;
    void* d;
    for (int i = 0; i < amount; i++)
    {
        d = da_remove_last(a);
        (*a->freeObject)(d);
    }
    return amount;
}

int da_insert(darray *a, void* item)
{
    if (a == NULL || a->array == NULL)
    {
        return NULLARG;
    }
    // memory increase
    if (a->used == a->size)
    { // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        void* *temp;
        temp = (void* *)realloc(a->array, sizeof(void*) * a->size);
        if (temp != NULL)
        {
            a->array = temp;
        }
        else
        {
            return ERRMEMR;
        }
    }
    a->used += 1;
    a->array[a->used - 1] = item;
    return a->used;
}

const void* da_at(darray * a, uint index)
{
    if (da_count(a) > index)
    {
        return a->array[index];
    }
    else
    {
        // out of bounds
        return NULL;
    }
}

void* da_remove_last(darray *a)
{
    if (da_is_empty(a))
    {
        return NULL;
    }
    a->used -= 1;
    MEM m = memory_decrease(a);
    if (m != ERRMEM_DECREASE)
    {
        void* data = a->array[a->used];
        return data;
    }
    else
    {
        a->used += 1; // rollback
        return NULL;
    }
}

void* da_remove_item(darray *a, void* item)
{
    uint count = da_count(a);
    for (uint i = 0; i < count; i++)
    {
        if ((*a->compare)(item, a->array[i]) == 0)
        { // compare with user defined function
            return da_remove_at(a, i);
        }
    }
    return NULL;
}

void* da_remove_at(darray *a, uint index)
{
    uint count = da_count(a);
    if (count <= index)
    {
        // out of bounds
        return NULL;
    }
    void* *data = a->array[index];
    for (uint i = index; i < count - 1; i++)
    {
        a->array[i] = a->array[i + 1];
    }
    a->used -= 1;
    MEM m = memory_decrease(a);
    if (m != ERRMEM_DECREASE)
    {
        return data;
    }
    else
    {
        a->used += 1;             // rollback
        a->array[a->used] = data; // "rollback"
        return NULL;
    }
}

int da_merge(darray *a, darray *b)
{
    if (a == NULL || b == NULL)
    {
        return NULLARG;
    }
    for (int i = 0; i < b->used; i++)
    {
        da_insert(a, b->array[i]);
    }
    // free only array not the elements (da_free frees all items in the array as well)
    free(b->array);
    b->size = 0;
    b->used = 0;
    b->array = NULL;
    return a->size;
}

bool da_is_null(darray *a)
{
    return a->array == NULL;
}

bool da_is_empty(darray *a)
{
    return a->used == 0;
}

// linear search
int da_find(darray *a, void* item)
{
    for (int i = 0; i < da_count(a); i++)
    {
        if ((*a->compare)(a->array[i], item))
        {
            return i;
        }
    }
    return -1;
}

bool da_exists(darray *a, void* item)
{
    if (da_find(a, item) != -1)
    {
        return true;
    }
    else
        return false;
}

// memory check and increase is done in da_insert function
static MEM memory_decrease(darray *a)
{
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if (ratio <= QUARTER && da_size(a) != 1)
    {
        a->size /= 2;
        void* *temp;
        temp = (void* *)realloc(a->array, sizeof(void*) * a->size);
        if (temp != NULL)
        {
            a->array = temp;
            return MEM_HALVED;
        }
        else
        {
            return ERRMEM_DECREASE;
        }
    }
    return MEM_MS_REACHED_NO_NEED;
}
