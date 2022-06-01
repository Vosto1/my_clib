#include "heap.h"
/*
* min-heap -> the smallest key is the root of each sub-tree
*/

heap createEmptyHeap() {
    heap h;
    h.compare = NULL;
    h.setKey = NULL;
    h.minKey = NULL;
    h.items = createEmptyDynamicArray();
    return h;
}

size_t initHeap(heap* h,
                size_t size,
                int (*compare)(cvoidp x, cvoidp y),
                voidp (*setKey)(voidp x, voidp key),
                void (*minKey)(voidp base, voidp* out)) {
    h->compare = compare;
    h->setKey = setKey;
    h->minKey = minKey;
    return initDynamicArray(&(h->items), size, compare);
}

void freeHeap(heap* h) {
    if (!h_is_null(h)) {
        freeArray(&h->items);
    } else { 
        errcset(EFREE_NULLPTR);
    }
}

size_t heapSize(heap* h) {
    return h->items.used;
}

/*
* return the item with the smallest key (the top of the heap).
* The item remains in the heap.
*/
voidp _min(heap* h) {
    return h->items.array[0];
}

bool h_is_null(heap* h) {
    return a_is_null(&h->items);
}

bool h_is_empty(heap* h) {
    return a_is_empty(&h->items);
}

/*
* Add to the heap.
*/
size_t heapInsert(heap* h, voidp item) {
    arrayInsert(&(h->items), item);
    if (errc != SUCCESS) {
        return -1;
    }
    minHeapifyUp(h, heapSize(h) - 1);
    return heapSize(h);
}

/*
* removes the item from the heap
* set key to a sufficiently small key using userdefined minKey function
* (example: minInt or current root key - 1) to give the item to be removed
* highest priority, then use extractMin
*/
voidp heapRemove(heap* h, voidp item) {
    if (heapSize(h) == 0) {
        return NULL; // if empty heap return NULL
    }
    voidp temp;
    (*h->minKey)(_min(h), &temp);
    if (decreaseKey(h, item, temp) == -1) {
        free(temp);
        return NULL; // error
    }
    free(temp);
    return extractMin(h);
}

/*
* Return the item with the smallest key (== highest priority).
* The item is also removed from the heap
*/
voidp extractMin(heap* h) {
    if (h_is_null(h)) {
        errcset(EHEAP_NULL);
        return NULL;
    }
    if (h_is_empty(h)) {
        errcset(EHEAP_EMPTY);
        return NULL;
    }
    voidp tempMin = h->items.array[0];
    voidp tempLast = h->items.array[heapSize(h) - 1];
    arrayRemoveAt(&(h->items), heapSize(h) - 1); // remove at last index
    if (heapSize(h) != 0) { // if the heap is not empty after removal
        h->items.array[0] = tempLast;
        minHeapifyDown(h, 0);
    }
    return tempMin;
}

/*
* PRECONDITION: newKey > item.Key and item already exists in the queue
* POSTCONDITION: x.Key == newKey
* Increases the items priority by assigning it a higher value Key.
* The properties of the data structure must be preserved.
*/
int decreaseKey(heap* h, voidp item, voidp newKey) {
    for (int i = 0; i < heapSize(h); i++) {
        if ((*h->compare)(h->items.array[i], item) == 0) {
            if ((*h->compare)(h->items.array[i], newKey) == 0) {
                errcset(EHNEW_KEY);
                return -1;
            }
            (*h->setKey)(h->items.array[i], newKey);
            int newindex = minHeapifyUp(h, i);
            if ((*h->compare)(h->items.array[0], newKey) != 0) {
                errcset(EHNEWKEY_NOT_SET);
                return -1;
            }
            return newindex;
        }
    }
    errcset(EH_DATA_DOESNT_EXIST);
    return -1;
}

/*
* start at index heapSize/2 (all elements after are leafs)
* go through the non-leafs "backwards" and heapify-down
* builds heap from an unordered list (array)
*/
heap buildMinHeap(voidp* unorderedList,
                size_t size,
                int (*compare)(cvoidp x, cvoidp y),
                voidp (*setKey)(voidp x, voidp key),
                void (*minKey)(voidp base, voidp* out)) {
    heap h;
    initHeap(&h, size, compare, setKey, minKey);
    for (int i = 0; i < size; i++)
        arrayInsert(&h.items, unorderedList[i]);
    for (int j = size / 2; j >= 0; j--)
        minHeapifyDown(&h, j);
    return h;
}

/*test if there are any heap violations*/
bool testHeapIntegrity(heap* h) {
    for (int i = 0; i < heapSize(h); i++) {
        int l = left(i);
        int r = right(i);
        voidp i1 = h->items.array[i];
        if (l < heapSize(h)) {
            voidp l1 = h->items.array[l];
            if ((*h->compare)(i1, l1) > 0)
            {
                return false;
            }
        }
        if (r < heapSize(h)) {
            voidp r1 = h->items.array[r];
            if ((*h->compare)(i1, r1) > 0) {
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
static int parent(int i) {
    return (i - 1) / 2;
}

/*given index for an item, returns index of its left child*/
static int left(int i) {
    return (2 * i) + 1;
}

/*given index for an item, returns index of its right child*/
static int right(int i) {
    return (2 * i) + 2;
}

/*swap two items in a dynamic array*/
static void swap(array* a, int i1, int i2) {
    voidp temp1 = a->array[i1];
    a->array[i1] = a->array[i2];
    a->array[i2] = temp1;
}

/*Maintains the heap properties*/
static int minHeapifyDown(heap* h, int index) {
    int l = left(index);
    int r = right(index);
    int smallest;
    if (l <= heapSize(h) - 1 && (*h->compare)(h->items.array[l], h->items.array[index]) < 0)
        smallest = l;
    else
        smallest = index;
    if (r <= heapSize(h) - 1 && (*h->compare)(h->items.array[r], h->items.array[smallest]) < 0)
        smallest = r;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        minHeapifyDown(h, smallest);
    } else return smallest; // return the last index
}
/*Maintains the heap properties*/
static int minHeapifyUp(heap* h, int index) {
    int p = parent(index);
    int smallest;
    if (p <= heapSize(h) - 1 && (*h->compare)(h->items.array[index], h->items.array[p]) < 0)
        smallest = p;
    else
        smallest = index;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        minHeapifyUp(h, smallest);
    } else return smallest; // return the last index
}