#include "s_heap.h"
/*
* min-heap -> the smallest key is the root of each sub-tree
* max-heap -> the largest key is the root of each sub-tree
*/
// (*h->compare)

heap s_createEmptyHeap() {
    heap h;
    h.compare = NULL;
    h.items = createEmptyDynamicArray();
    return h;
}

size_t s_initHeap(heap* h, size_t size, int (*compare)(Data x, Data y)) {
    h->compare = compare;
    return initDynamicArray(&(h->items), size, compare);
}

void s_freeHeap(heap* h) {
    if (!sh_is_null(h)) {
        freeArray(&h->items);
    } else { 
        errcset(EFREE_NULLPTR);
    }
}

size_t s_heapSize(heap* h) {
    return h->items.used;
}

/*
* return the item with the smallest key (== highest priority).
* The item remains in the queue.
*/
Data s_min(heap* h) {
    return h->items.array[0];
}

bool sh_is_null(heap* h) {
    return a_is_null(&h->items);
}

bool sh_is_empty(heap* h) {
    return a_is_empty(&h->items);
}

/*
* Add to the queue.
*/
size_t s_heapInsert(heap* h, Data item) {
    arrayInsert(&(h->items), item);
    if (errc != SUCCESS) {
        return -1;
    }
    s_minHeapifyUp(h, s_heapSize(h) - 1);
    return h->items.used;
}

/*
* Return the item with the smallest key (== highest priority).
* The item is also removed from the queue
*/
Data s_extractMin(heap* h) {
    if (sh_is_empty(h)) {
        errcset(EHEAP_EMPTY);
        return NULL;
    }
    Data tempMin = h->items.array[0];
    Data tempLast = h->items.array[s_heapSize(h) - 1];
    arrayRemoveAt(&(h->items), s_heapSize(h) - 1); // remove at last index
    if (s_heapSize(h) != 0) { // if the heap is not empty after removal
        h->items.array[0] = tempLast;
        s_minHeapifyDown(h, 0);
    }
    return tempMin;
}

/*
* Howto: start at index heapSize/2 (all elements after are leafs) go through the non-leafs "backwards" and heapify-down
* builds heap from an unordered list (array)
*/
heap s_buildMinHeap(Data* unorderedList, size_t size, int (*compare)(Data x, Data y)) {
    heap h;
    s_initHeap(&h, size, compare);
    for (int i = 0; i < size; i++)
        arrayInsert(&h.items, unorderedList[i]);
    for (int j = size / 2; j >= 0; j--)
        s_minHeapifyDown(&h, j);
    return h;
}

bool s_testHeapIntegrity(heap* h) {
    for (int i = 0; i < s_heapSize(h); i++) {
        int l = s_left(i);
        int r = s_right(i);
        Data i1 = h->items.array[i];
        if (l < s_heapSize(h)) {
            Data l1 = h->items.array[l];
            if ((*h->compare)(i1, l1) > 0)
            {
                return false;
            }
        }
        if (r < s_heapSize(h)) {
            Data r1 = h->items.array[r];
            if ((*h->compare)(i1, r1) > 0) {
                return false;
            }
        }
    }
    return true;
}

/*
* Guide for index calculation:
* https://stackoverflow.com/questions/22900388/why-in-a-heap-implemented-by-array-the-index-0-is-left-unused 
*/
/*given index for an item, returns index of parent*/
int s_parent(int i) {
    return (i - 1) / 2;
}

/*given index for an item, returns index of its left child*/
int s_left(int i) {
    return (2 * i) + 1;
}

/*given index for an item, returns index of its right child*/
int s_right(int i) {
    return (2 * i) + 2;
}

void s_swap(array* a, int i1, int i2) {
    Data temp1 = a->array[i1];
    a->array[i1] = a->array[i2];
    a->array[i2] = temp1;
}

/*Maintains the heap properties*/
void s_minHeapifyDown(heap* h, int index)
{
    int l = s_left(index);
    int r = s_right(index);
    int smallest;
    if (l <= s_heapSize(h) - 1 && (*h->compare)(h->items.array[l], h->items.array[index]) < 0)
        smallest = l;
    else
        smallest = index;
    if (r <= s_heapSize(h) - 1 && (*h->compare)(h->items.array[r], h->items.array[smallest]) < 0)
        smallest = r;
    if (smallest != index)
    {
        s_swap(&(h->items), index, smallest);
        s_minHeapifyDown(h, smallest);
    }
}
/*Maintains the heap properties*/
void s_minHeapifyUp(heap* h, int index)
{
    int p = s_parent(index);
    int smallest;
    if (p <= s_heapSize(h) - 1 && (*h->compare)(h->items.array[index], h->items.array[p]) < 0)
        smallest = p;
    else
        smallest = index;
    if (smallest != index)
    {
        s_swap(&(h->items), index, smallest);
        s_minHeapifyUp(h, smallest);
    }
}