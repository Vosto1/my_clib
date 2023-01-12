#include "heap.h"
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
static void swap(darray *a, uint i1, uint i2);
/**
 * @brief compare parent with children, if there is a heap violation then switch parent and child then recursively continue down the heap
 *
 * @param h heap to heapify
 * @param index index to start on
 */
static uint min_heapify_down(heap *h, uint index);
/**
 * @brief compare child with parent, if there is a heap violation then swap the child and the parent and then recursively continue up the heap
 *
 * @param h heap to heapify
 * @param index index to start on
 */
static uint min_heapify_up(heap *h, uint index);

heap h_create_empty()
{
    heap h;
    h.compare = NULL;
    h.setKey = NULL;
    h.minKey = NULL;
    h.items = da_create_empty();
    return h;
}

int h_init(heap *h,
                uint size,
                int (*compare)(const void* x, const void* y),
                void* (*setKey)(void* x, void* key),
                void (*minKey)(void* base, void* *out),
                void (*freeObject)(void*))
{
    h->compare = compare;
    h->setKey = setKey;
    h->minKey = minKey;
    return da_init(&(h->items), size, compare, freeObject);
}

bool h_free(heap *h)
{
    if (!h_is_null(h))
    {
        da_free(&h->items);
        return true;
    }
    return false;
}

uint h_size(heap *h)
{
    return da_count(&h->items);
}

/*
 * return the item with the smallest key (the top of the heap).
 * The item remains in the heap.
 */
void* h_min(heap *h)
{
    return h->items.array[0];
}

bool h_is_null(heap *h)
{
    return da_is_null(&h->items);
}

bool h_is_empty(heap *h)
{
    return da_is_empty(&h->items);
}


/*
 * Add to the heap.
 */
int h_insert(heap *h, void* item)
{
    int s = da_count(&h->items);
    int s2 = da_insert(&(h->items), item);
    if (s + 1 != s2)
    {
        return ERROPERATION;
    }
    min_heapify_up(h, h_size(h) - 1);
    return h_size(h);
}

/*
 * removes the item from the heap
 * set key to a sufficiently small key using userdefined minKey function
 * (example: minInt or current root key - 1) to give the item to be removed
 * highest priority, then use h_extract_min
 */
void* h_remove(heap *h, void* item)
{
    if (h_size(h) == 0)
    {
        return NULL; // if empty heap return NULL
    }
    void* temp;
    (*h->minKey)(h_min(h), &temp);
    if (h_decrease_key(h, item, temp) > h_size(h))
    {
        free(temp);
        return NULL; // error
    }
    free(temp);
    return h_extract_min(h);
}

/*
 * Return the item with the smallest key (== highest priority).
 * The item is also removed from the heap
 */
void* h_extract_min(heap *h)
{
    if (h_is_null(h))
    {
        //errcset(EHEAP_NULL);
        return NULL;
    }
    if (h_is_empty(h))
    {
        //errcset(EHEAP_EMPTY);
        return NULL;
    }
    void* tempMin = h->items.array[0];
    void* tempLast = h->items.array[h_size(h) - 1];
    da_remove_at(&(h->items), h_size(h) - 1); // remove at last index
    if (h_size(h) != 0)
    { // if the heap is not empty after removal
        h->items.array[0] = tempLast;
        min_heapify_down(h, 0);
    }
    return tempMin;
}

/*
 * PRECONDITION: newKey > item.Key and item already exists in the queue
 * POSTCONDITION: x.Key == newKey
 * Increases the items priority by assigning it a higher value Key.
 * The properties of the data structure must be preserved.
 */
int h_decrease_key(heap *h, void* item, void* newKey)
{
    for (int i = 0; i < h_size(h); i++)
    {
        if ((*h->compare)(h->items.array[i], item) == 0)
        {
            if ((*h->compare)(h->items.array[i], newKey) == 0)
            {
                return SAMEKEY;
            }
            (*h->setKey)(h->items.array[i], newKey);
            int newindex = min_heapify_up(h, i);
            if ((*h->compare)(h->items.array[0], newKey) != 0)
            {
                return KEYNOTSET;
            }
            return newindex;
        }
    }
    return DATADOESNTEXIST;
}

/*
 * start at index heapSize/2 (all elements after are leafs)
 * go through the non-leafs "backwards" and heapify-down
 * builds heap from an unordered list (array)
 */
heap h_build_min_heap(void* *unorderedList,
                  int size,
                  int (*compare)(const void* x, const void* y),
                  void* (*setKey)(void* x, void* key),
                  void (*minKey)(void* base, void* *out),
                  void (*freeObject)(void*))
{
    heap h;
    h_init(&h, size, compare, setKey, minKey, freeObject);
    for (int i = 0; i < size; i++)
        da_insert(&h.items, unorderedList[i]);
    for (int j = size / 2; j >= 0; j--)
        min_heapify_down(&h, j);
    return h;
}

/*test if there are any heap violations*/
bool test_heap_integrity(heap *h)
{
    for (int i = 0; i < h_size(h); i++)
    {
        int l = left(i);
        int r = right(i);
        void* i1 = h->items.array[i];
        if (l < h_size(h))
        {
            void* l1 = h->items.array[l];
            if ((*h->compare)(i1, l1) > 0)
            {
                return false;
            }
        }
        if (r < h_size(h))
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
static void swap(darray *a, uint i1, uint i2)
{
    void* temp1 = a->array[i1];
    a->array[i1] = a->array[i2];
    a->array[i2] = temp1;
}

/*Maintains the heap properties*/
static uint min_heapify_down(heap *h, uint index)
{
    int l = left(index);
    int r = right(index);
    int smallest;
    if (l <= h_size(h) - 1 && (*h->compare)(h->items.array[l], h->items.array[index]) < 0)
        smallest = l;
    else
        smallest = index;
    if (r <= h_size(h) - 1 && (*h->compare)(h->items.array[r], h->items.array[smallest]) < 0)
        smallest = r;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        return min_heapify_down(h, smallest);
    }
    else
        return smallest; // return the last index
}
/*Maintains the heap properties*/
static uint min_heapify_up(heap *h, uint index)
{
    int p = parent(index);
    int smallest;
    if (p <= h_size(h) - 1 && (*h->compare)(h->items.array[index], h->items.array[p]) < 0)
        smallest = p;
    else
        smallest = index;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        return min_heapify_up(h, smallest);
    }
    else
        return smallest; // return the last index
}