#include "sdarray.h"

static MEM sda_memory_decrease(sdarray *a);

size_t sda_count(sdarray *a)
{
    return a->used;
}

size_t sda_size(sdarray *a)
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

size_t sda_init(sdarray *a, size_t init_size, void (*freeObject)(void*))
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
        //errcset(EMEM_ALLOC);
        return 0;
    }
}

size_t sda_clear(sdarray *a)
{
    if (sda_is_empty(a))
    {
        return 1; // its empty job already done
    }
    else if (sda_is_null(a))
    {
        //errcset(ENULL_ARG);
        return 0;
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
    /* else
    {
        errcset(EFREE_NULLPTR);
    }*/
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
    //errcset(EFREE_NULLPTR);
    return false;
}

const void* sda_at(sdarray * a, size_t index)
{
    if (sda_count(a) < index)
    {
        return a->array[index];
    }
    else
    {
        // error
        return NULL;
    }
}

size_t sda_insert(sdarray *a, void* item)
{
    if (a == NULL || a->array == NULL)
    {
        //errcset(ENULL_ARG);
        return 0;
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
            //errcset(EMEM_IREALLOC);
            return 0;
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
        //errcset(EARR_EMPTY);
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

void* sda_remove_at(sdarray *a, int index)
{
    if (index > a->used)
    {
        //errcset(EINDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    void* *data = a->array[index];
    for (int i = index; i < a->used; i++)
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

size_t sda_merge(sdarray *a, sdarray *b)
{
    if (a == NULL || b == NULL)
    {
        //errcset(ENULL_ARG);
        return 0;
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
            //errcset(EMEM_DREALLOC);
            return ERRMEM_DECREASE;
        }
    }
    return MEM_MS_REACHED_NO_NEED;
}