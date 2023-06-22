#include "quicksort.h"

static void swap(void* a[], uint i, uint j)
{
    void* temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

static uint sort_partition(void* array[], uint start, uint end, int (*compare)(void*, void*))
{
    uint pivot = start;
    uint store_index = pivot + 1;
    for (uint i = start + 1; i < end; i++)
    {
        if ((*compare)(array[i], array[pivot]) <= 0)
        {
            swap(array, i, store_index);
            store_index++;
        }
    }
    swap(array, pivot, store_index - 1);
    return store_index - 1;
}

static void sort(void* array[], uint start, uint end, int (*compare)(void*, void*))
{
    if (start < end)
    {
        uint old_pivot = sort_partition(array, start, end, compare);
        sort(array, start, old_pivot, compare);
        sort(array, old_pivot+1, end, compare);
    }
}

void quicksort(void* array[], uint array_size, int (*compare)(void*, void*))
{
    sort(array, 0, array_size, compare);
}