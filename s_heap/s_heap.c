#include "s_heap.h"
/*
 * min-heap -> the smallest key is the root of each sub-tree
 */

/**
 * @brief get the parent index of another index
 *
 * @param i index to get parent off of
 * @return parent index
 */
static int parent(int i);
/**
 * @brief get the left index of another index
 *
 * @param i index to get left off of
 * @return left index
 */
static int left(int i);
/**
 * @brief get the right index of another index
 *
 * @param i index to get right off of
 * @return right index
 */
static int right(int i);
/**
 * @brief swap the item on index i1 with the item on index i2
 *
 * @param a the array to swap items in
 * @param i1 index of item 1
 * @param i2 index of item 2
 */
static void swap(darray *a, int i1, int i2);
/**
 * @brief compare parent with children, if there is a heap violation then switch parent and child then recursively continue down the sheap
 *
 * @param h sheap to heapify
 * @param index index to start on
 */
static void min_heapify_down(sheap *h, int index);
/**
 * @brief compare child with parent, if there is a heap violation then swap the child and the parent and then recursively continue up the sheap
 *
 * @param h sheap to heapify
 * @param index index to start on
 */
static void min_heapify_up(sheap *h, int index);

sheap sh_create_empty()
{
    sheap h;
    h.compare = NULL;
    h.items = da_create_empty();
    return h;
}

size_t sh_init(sheap *h, size_t size, int (*compare)(cvoidp_t x, cvoidp_t y), void (*freeObject)(voidp_t))
{
    h->compare = compare;
    return da_init(&(h->items), size, compare, freeObject);
}

bool sh_free(sheap *h)
{
    if (!sh_is_null(h))
    {
        return da_free(&h->items);
    }
    return false;
    /* else
    {
        errcset(EFREE_NULLPTR);
    } */
}

size_t sh_size(sheap *h)
{
    return h->items.used;
}

/*
 * return the item with the smallest key (the top of the heap).
 * The item remains in the heap.
 */
voidp_t sh_peek(sheap *h)
{
    return h->items.array[0];
}

bool sh_is_null(sheap *h)
{
    return da_is_null(&h->items);
}

bool sh_is_empty(sheap *h)
{
    return da_is_empty(&h->items);
}

#define FLAG_ERROR 1000

/*
 * Add to the heap.
 */
size_t sh_insert(sheap *h, voidp_t item)
{
    size_t s = da_count(&(h->items));
    size_t s2 = da_insert(&(h->items), item);
    if (s + 1 != s2)
    {
        return sh_size(h) + FLAG_ERROR;
    }
    min_heapify_up(h, sh_size(h) - 1);
    return h->items.used;
}

/*
 * Return the item with the smallest key (== highest priority).
 * The item is also removed from the heap
 */
voidp_t sh_extract_min(sheap *h)
{
    if (sh_is_null(h))
    {
        //errcset(EHEAP_NULL);
        return NULL;
    }
    if (sh_is_empty(h))
    {
        //errcset(EHEAP_EMPTY);
        return NULL;
    }
    voidp_t tempMin = h->items.array[0];
    voidp_t tempLast = h->items.array[sh_size(h) - 1];
    da_remove_at(&(h->items), sh_size(h) - 1); // remove at last index
    if (sh_size(h) != 0)
    { // if the heap is not empty after removal
        h->items.array[0] = tempLast;
        min_heapify_down(h, 0);
    }
    return tempMin;
}

/*
 * start at index heapSize/2 (all elements after are leafs)
 * go through the non-leafs "backwards" and heapify-down
 * builds heap from an unordered list (array)
 */
sheap sh_build_min_heap(voidp_t *unorderedList, size_t size, int (*compare)(cvoidp_t x, cvoidp_t y), void (*freeObject)(voidp_t))
{
    sheap h;
    sh_init(&h, size, compare, freeObject);
    for (int i = 0; i < size; i++)
        da_insert(&h.items, unorderedList[i]);
    for (int j = size / 2; j >= 0; j--)
        min_heapify_down(&h, j);
    return h;
}

/*test if there are any heap violations*/
bool sh_test_heap_integrity(sheap *h)
{
    for (int i = 0; i < sh_size(h); i++)
    {
        int l = left(i);
        int r = right(i);
        voidp_t i1 = h->items.array[i];
        if (l < sh_size(h))
        {
            voidp_t l1 = h->items.array[l];
            if ((*h->compare)(i1, l1) > 0)
            {
                return false;
            }
        }
        if (r < sh_size(h))
        {
            voidp_t r1 = h->items.array[r];
            if ((*h->compare)(i1, r1) > 0)
            {
                return false;
            }
        }
    }
    return true;
}

/*
 * index calculation:
 * https://stackoverflow.com/questions/22900388/why-in-a-heap-implemented-by-array-the-index-0-is-left-unused
 */
/*given index for an item, returns index of parent*/
static int parent(int i)
{
    return (i - 1) / 2;
}

/*given index for an item, returns index of its left child*/
static int left(int i)
{
    return (2 * i) + 1;
}

/*given index for an item, returns index of its right child*/
static int right(int i)
{
    return (2 * i) + 2;
}

/*swap two items in a dynamic array*/
static void swap(darray *a, int i1, int i2)
{
    voidp_t temp1 = a->array[i1];
    a->array[i1] = a->array[i2];
    a->array[i2] = temp1;
}

/*Maintains the heap properties*/
static void min_heapify_down(sheap *h, int index)
{
    int l = left(index);
    int r = right(index);
    int smallest;
    if (l <= sh_size(h) - 1 && (*h->compare)(h->items.array[l], h->items.array[index]) < 0)
        smallest = l;
    else
        smallest = index;
    if (r <= sh_size(h) - 1 && (*h->compare)(h->items.array[r], h->items.array[smallest]) < 0)
        smallest = r;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        min_heapify_down(h, smallest);
    }
}
/*Maintains the heap properties*/
static void min_heapify_up(sheap *h, int index)
{
    int p = parent(index);
    int smallest;
    if (p <= sh_size(h) - 1 && (*h->compare)(h->items.array[index], h->items.array[p]) < 0)
        smallest = p;
    else
        smallest = index;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        min_heapify_up(h, smallest);
    }
}