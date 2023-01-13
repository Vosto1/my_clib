#include "sdarray.h"

#define QUARTER 0.25

typedef enum
{                               // decrease memory results
    MEM_HALVED = 0,             // memory halved
    MEM_OK = 1,                 // memory min size reached or no need
    ERRMEM_DECREASE = 2,        // ERRor MEMory DECREASE
} MEM;

static MEM sda_memory_decrease(sdarray *a);

uint sda_count(sdarray *a)
{
    return a->used;
}

uint sda_size(sdarray *a)
{
    return a->size;
}

sdarray sda_create_empty()
{
    sdarray a;
    a.array = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

int sda_init(sdarray *a, uint init_size, void (*freeObject)(void*))
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
        *a = sda_create_empty();
        return ERRMEM;
    }
}

int sda_clear(sdarray *a)
{
    if (sda_is_empty(a))
    {
        return 1; // its empty, job already done
    }
    else if (sda_is_null(a))
    {
        return NULLARG;
    }
    int amount = a->used;
    void* d;
    for (int i = 0; i < amount; i++)
    {
        d = sda_remove_last(a);
        (*a->freeObject)(d);
    }
    return amount;
}

bool sda_destroy(sdarray *a)
{
    if (!sda_is_null(a))
    {
        free(a->array);
        *a = sda_create_empty();
        return true;
    }
    return false;
}

bool sda_free(sdarray *a)
{
    if (!sda_is_null(a))
    {
        if (!sda_is_empty(a))
        {
            sda_clear(a);
            sda_destroy(a);
        }
        else
            sda_destroy(a);
        return true;
    }
    return false;
}

const void* sda_at(sdarray * a, uint index)
{
    if (sda_count(a) > index)
    {
        return a->array[index];
    }
    else
    {
        // out of bounds
        return NULL;
    }
}

int sda_insert(sdarray *a, void* item)
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

void* sda_remove_last(sdarray *a)
{
    if (sda_is_empty(a))
    {
        return NULL;
    }
    a->used -= 1;
    void* data = a->array[a->used];
    MEM m = sda_memory_decrease(a);
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

void* sda_remove_at(sdarray *a, uint index)
{
    uint count = sda_count(a);
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
    MEM m = sda_memory_decrease(a);
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

int sda_merge(sdarray *a, sdarray *b)
{
    if (a == NULL || b == NULL)
    {
        return NULLARG;
    }
    for (int i = 0; i < b->used; i++)
    {
        sda_insert(a, b->array[i]);
    }
    // free only array not the elements (sda_free frees all items in the array as well)
    sda_destroy(b);
    return a->size;
}

bool sda_is_null(sdarray *a)
{
    return a->array == NULL;
}

bool sda_is_empty(sdarray *a)
{
    return a->used == 0;
}

// memory check + increase is done in arrayInsert function
static MEM sda_memory_decrease(sdarray *a)
{
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if (ratio <= QUARTER && sda_size(a) != 1)
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