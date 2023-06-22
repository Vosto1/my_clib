#include "mheap.h"
/*
 * min-heap -> the smallest key is the root of each sub-tree
 */

/**
 * @brief get the parent index of another index
 *
 * @param i index to get parent off of
 * @return parent index
 */
static uint parent(uint i);
/**
 * @brief get the left index of another index
 *
 * @param i index to get left off of
 * @return left index
 */
static uint left(uint i);
/**
 * @brief get the right index of another index
 *
 * @param i index to get right off of
 * @return right index
 */
static uint right(uint i);
/**
 * @brief swap the item on index i1 with the item on index i2
 *
 * @param a the array to swap items in
 * @param i1 index of item 1
 * @param i2 index of item 2
 */
static void swap(mdarray *a, uint i1, uint i2);
/**
 * @brief compare parent with children, if there is a heap violation then switch parent and child then recursively continue down the mheap
 *
 * @param h mheap to heapify
 * @param index index to start on
 */
static void min_heapify_down(mheap *h, uint index);
/**
 * @brief compare child with parent, if there is a heap violation then swap the child and the parent and then recursively continue up the mheap
 *
 * @param h mheap to heapify
 * @param index index to start on
 */
static void min_heapify_up(mheap *h, uint index);

mheap mh_create_empty()
{
    mheap h;
    h.compare = NULL;
    h.items = mda_create_empty();
    return h;
}

int mh_init(mheap *h, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*))
{
    h->compare = compare;
    return mda_init(&(h->items), size, freeObject);
}

bool mh_free(mheap *h)
{
    if (!mh_is_null(h))
    {
        return mda_free(&h->items);
    }
    return false;
}

bool mh_destroy(mheap* h)
{
    if (!mh_is_null(h))
    {
        return mda_destroy(&h->items);
    }
    return false;
}

uint mh_size(mheap *h)
{
    return mda_count(&h->items);
}

/*
 * return the item with the smallest key (the top of the heap).
 * The item remains in the heap.
 */
void* mh_peek(mheap *h)
{
    return h->items.array[0];
}

bool mh_is_null(mheap *h)
{
    return mda_is_null(&h->items);
}

bool mh_is_empty(mheap *h)
{
    return mda_is_empty(&h->items);
}


/*
 * Add to the heap.
 */
int mh_insert(mheap *h, void* item)
{
    int s = mda_count(&(h->items));
    int s2 = mda_insert(&(h->items), item);
    if (s + 1 != s2)
    {
        return ERROPERATION;
    }
    min_heapify_up(h, mh_size(h) - 1);
    return h->items.used;
}

/*
 * Return the item with the smallest key (== highest priority).
 * The item is also removed from the heap
 */
void* mh_extract_min(mheap *h)
{
    if (mh_is_null(h))
    {
        // null arg
        return NULL;
    }
    if (mh_is_empty(h))
    {
        // empty heap
        return NULL;
    }
    void* tempMin = h->items.array[0];
    void* tempLast = h->items.array[mh_size(h) - 1];
    mda_remove_at(&(h->items), mh_size(h) - 1); // remove at last index
    if (mh_size(h) != 0)
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
mheap mh_build_min_heap(void* *unorderedList, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*))
{
    mheap h;
    mh_init(&h, size, compare, freeObject);
    for (uint i = 0; i < size; i++)
        mda_insert(&h.items, unorderedList[i]);
    for (int j = size / 2; j >= 0; j--)
        min_heapify_down(&h, j);
    return h;
}

/*test if there are any heap violations*/
bool mh_test_heap_integrity(mheap *h)
{
    for (uint i = 0; i < mh_size(h); i++)
    {
        uint l = left(i);
        uint r = right(i);
        void* i1 = h->items.array[i];
        if (l < mh_size(h))
        {
            void* l1 = h->items.array[l];
            if ((*h->compare)(i1, l1) > 0)
            {
                return false;
            }
        }
        if (r < mh_size(h))
        {
            void* r1 = h->items.array[r];
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
static uint parent(uint i)
{
    return (i - 1) / 2;
}

/*given index for an item, returns index of its left child*/
static uint left(uint i)
{
    return (2 * i) + 1;
}

/*given index for an item, returns index of its right child*/
static uint right(uint i)
{
    return (2 * i) + 2;
}

/*swap two items in a dynamic array*/
static void swap(mdarray *a, uint i1, uint i2)
{
    void* temp1 = a->array[i1];
    a->array[i1] = a->array[i2];
    a->array[i2] = temp1;
}

/*Maintains the heap properties*/
static void min_heapify_down(mheap *h, uint index)
{
    uint l = left(index);
    uint r = right(index);
    uint smallest;
    if (l <= mh_size(h) - 1 && (*h->compare)(h->items.array[l], h->items.array[index]) < 0)
        smallest = l;
    else
        smallest = index;
    if (r <= mh_size(h) - 1 && (*h->compare)(h->items.array[r], h->items.array[smallest]) < 0)
        smallest = r;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        min_heapify_down(h, smallest);
    }
}
/*Maintains the heap properties*/
static void min_heapify_up(mheap *h, uint index)
{
    uint p = parent(index);
    uint smallest;
    if (p <= mh_size(h) - 1 && (*h->compare)(h->items.array[index], h->items.array[p]) < 0)
        smallest = p;
    else
        smallest = index;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        min_heapify_up(h, smallest);
    }
}