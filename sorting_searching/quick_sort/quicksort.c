#include "quicksort.h"

static void swap(voidp_t a[], size_t i, size_t j)
{
    voidp_t temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

static size_t sort_partition(voidp_t array[], size_t start, size_t end, int (*compare)(voidp_t, voidp_t))
{
    size_t pivot = start;
    size_t store_index = pivot + 1;
    for (size_t i = start + 1; i < end; i++)
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

static void sort(voidp_t array[], size_t start, size_t end, int (*compare)(voidp_t, voidp_t))
{
    if (start < end)
    {
        size_t old_pivot = sort_partition(array, start, end, compare);
        sort(array, start, old_pivot, compare);
        sort(array, old_pivot+1, end, compare);
    }
}

void quicksort(voidp_t array[], size_t array_size, int (*compare)(voidp_t, voidp_t))
{
    sort(array, 0, array_size, compare);
}