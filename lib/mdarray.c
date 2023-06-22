#include "mdarray.h"

#define QUARTER 0.25

typedef enum
{                               // decrease memory results
    MEM_HALVED = 0,             // memory halved
    MEM_OK = 1,                 // memory min size reached or no need
    ERRMEM_DECREASE = 2,        // ERRor MEMory DECREASE
} MEM;

static MEM mda_memory_decrease(mdarray *a);

uint mda_count(mdarray *a)
{
    return a->used;
}

uint mda_size(mdarray *a)
{
    return a->size;
}

mdarray mda_create_empty()
{
    mdarray a;
    a.array = NULL;
    a.freeObject = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

int mda_init(mdarray *a, uint init_size, void (*freeObject)(void*))
{
    a->size = 0;
    a->used = 0;
    void* *temp;
    temp = (void* *)malloc(sizeof(void*) * init_size);
    if (temp != NULL)
    {
        a->array = temp;
        a->size = init_size;
        a->freeObject = freeObject;
        return init_size;
    }
    else
    {
        *a = mda_create_empty();
        return ERRMEM;
    }
}

int mda_clear(mdarray *a)
{
    if (mda_is_empty(a))
    {
        return 1; // its empty, job already done
    }
    else if (mda_is_null(a))
    {
        return NULLARG;
    }
    int amount = a->used;
    void* d;
    for (int i = 0; i < amount; i++)
    {
        d = mda_remove_last(a);
        (*a->freeObject)(d);
    }
    return amount;
}

bool mda_destroy(mdarray *a)
{
    if (!mda_is_null(a))
    {
        free(a->array);
        *a = mda_create_empty();
        return true;
    }
    return false;
}

bool mda_free(mdarray *a)
{
    if (!mda_is_null(a))
    {
        if (!mda_is_empty(a))
        {
            mda_clear(a);
            mda_destroy(a);
        }
        else
            mda_destroy(a);
        return true;
    }
    return false;
}

const void* mda_at(mdarray * a, uint index)
{
    if (mda_count(a) > index)
    {
        return a->array[index];
    }
    else
    {
        // out of bounds
        return NULL;
    }
}

int mda_insert(mdarray *a, void* item)
{
    if (a == NULL || a->array == NULL)
    {
        return NULLARG;
    }
    // memory increase
    if (a->used == a->size)
    {   // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        void* *temp = (void* *)realloc(a->array, sizeof(void*) * a->size);
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

void* mda_remove_last(mdarray *a)
{
    if (mda_is_empty(a))
    {
        return NULL;
    }
    a->used -= 1;
    void* data = a->array[a->used];
    MEM m = mda_memory_decrease(a);
    if (m != ERRMEM_DECREASE)
    {
        return data;
    }
    else
    {
        a->used += 1; // rollback
        return NULL;
    }
}

void* mda_remove_at(mdarray *a, uint index)
{
    uint count = mda_count(a);
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
    MEM m = mda_memory_decrease(a);
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

int mda_merge(mdarray *a, mdarray *b)
{
    if (a == NULL || b == NULL)
    {
        return NULLARG;
    }
    for (uint i = 0; i < b->used; i++)
    {
        mda_insert(a, b->array[i]);
    }
    // free only array not the elements (mda_free frees all items in the array as well)
    mda_destroy(b);
    return a->size;
}

bool mda_is_null(mdarray *a)
{
    return a->array == NULL;
}

bool mda_is_empty(mdarray *a)
{
    return a->used == 0;
}

// memory check + increase is done in arrayInsert function
static MEM mda_memory_decrease(mdarray *a)
{
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if (ratio <= QUARTER && mda_size(a) != 1)
    {
        a->size /= 2;
        void* *temp = (void* *)realloc(a->array, sizeof(void*) * a->size);
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
    return MEM_OK;
}