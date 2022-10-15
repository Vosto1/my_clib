#include "darray.h"

/**
 * checks if the memory should be halved and does so if check is positive
 *
 * @param a to decrease memory for
 * @return result of memory reduction
 */
static MEM memory_decrease(darray *a);

size_t da_count(darray *a)
{
    return a->used;
}

size_t da_size(darray *a)
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

size_t da_init(darray *a, size_t initSize, int (*compare)(cvoidp_t x, cvoidp_t y), void (*freeObject)(voidp_t))
{
    a->size = 0;
    a->used = 0;
    voidp_t *temp;
    temp = (voidp_t *)malloc(sizeof(voidp_t) * initSize);
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
        //errcset(EMEM_ALLOC);
        return 0;
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
    /* else
    {
        errcset(EFREE_NULLPTR);
    } */
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
    /* else
    {
        errcset(EFREE_NULLPTR);
    } */
}

size_t da_clear(darray *a)
{
    if (da_is_empty(a))
    {
        return 1;  // its empty job already done
    }
    else if (da_is_null(a))
    {
        //errcset(ENULL_ARG);
        return 0;
    }
    int amount = a->used;
    voidp_t d;
    for (int i = 0; i < amount; i++)
    {
        d = da_remove_last(a);
        (*a->freeObject)(d);
    }
    return amount;
}

size_t da_insert(darray *a, voidp_t item)
{
    if (a == NULL || a->array == NULL)
    {
        //errcset(ENULL_ARG);
        return 0;
    }
    // memory increase
    if (a->used == a->size)
    { // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        voidp_t *temp;
        temp = (voidp_t *)realloc(a->array, sizeof(voidp_t) * a->size);
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

voidp_t da_remove_last(darray *a)
{
    if (da_is_empty(a))
    {
        //errcset(EARR_EMPTY);
        return NULL;
    }
    a->used -= 1;
    MEM m = memory_decrease(a);
    if (m != ERRMEM_DECREASE)
    {
        voidp_t data = a->array[a->used];
        return data;
    }
    else
    {
        a->used += 1; // rollback
        return NULL;
    }
}

voidp_t da_remove_item(darray *a, voidp_t item)
{
    for (int i = 0; i < a->used; i++)
    {
        if ((*a->compare)(item, a->array[i]) == 0)
        { // compare with user defined function
            return da_remove_at(a, i);
        }
    }
    //errcset(EARRDATA_DOESNT_EXIST);
    return NULL;
}

voidp_t da_remove_at(darray *a, int index)
{
    if (index > a->used)
    {
        //errcset(EINDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    voidp_t *data = a->array[index];
    for (int i = index; i < a->used - 1; i++)
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
        a->array[a->used] = data; // rollback
        return NULL;
    }
}

size_t da_merge(darray *a, darray *b)
{
    if (a == NULL || b == NULL)
    {
        //errcset(ENULL_ARG);
        return 0;
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
int find(darray *a, voidp_t item)
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

bool da_exists(darray *a, voidp_t item)
{
    if (find(a, item) != -1)
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
        voidp_t *temp;
        temp = (voidp_t *)realloc(a->array, sizeof(voidp_t) * a->size);
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
