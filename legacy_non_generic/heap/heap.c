#include "heap.h"
/*
* min-heap -> the smallest key is the root of each sub-tree
* max-heap -> the largest key is the root of each sub-tree
*/


size_t heapSize(array* a) {
    return a->used;
}

void heapPrint(array* a) {
    printData(a->used, a->array);
}

heap createEmptyHeap() {
    heap h;
    return h;
}

ErrorCode initHeap(heap* h, size_t size) {
    return initDynamicArray(&(h->items), size);
}

/*
* Guide for index calculation:
* https://stackoverflow.com/questions/22900388/why-in-a-heap-implemented-by-array-the-index-0-is-left-unused 
*/
/*given index for an item, returns index of parent*/
int parent(int i) {
    return (i - 1) / 2;
}

/*given index for an item, returns index of its left child*/
int left(int i) {
    return (2 * i) + 1;
}

/*given index for an item, returns index of its right child*/
int right(int i) {
    return (2 * i) + 2;
}

/*
* Add to the queue.
*/
ErrorCode heapInsert(heap* h, Data item) {
    ErrorCode e;
    e = arrayInsert(&(h->items), item);
    if (e == SUCCESS) {
        minHeapifyUp(h, heapSize(&h->items) - 1);
        return SUCCESS;
    }
    else return e;
}

// guarantees the smallest key unless the heap has been changed after
Key minKey(heap* h) {
    Data min_key = _min(h);
    return decrementKey(min_key.key);
}

/*
* PRECONDITIONS: the item is in the queue
* removes the item from the queue
* Use increaseKey on the item with a sufficiently large key (example: maxInt or current root+1)
* to give the item to be removed highest priority, then use extractMax (and discard the return-value)
*/
bool heapRemove(heap* h, Data item) {
    if (heapSize(&h->items) == 0) {
        return false; // if empty heap return false
    }
    ErrorCode e = SUCCESS;
    Data temp;
    temp.element = 0;
    temp.key = minKey(h);
    e = decreaseKey(h, item, temp);
    if (e != SUCCESS)
        return false; // DATA_EXIST
    Data tmp = extractMin(h);
    return true;
}

/*
* Return the item with the smallest key (== highest priority).
* The item is also removed from the queue
*/
Data extractMin(heap* h) {
    Data tempMin = h->items.array[0];
    Data tempLast = h->items.array[heapSize(&h->items) - 1];
    arrayRemoveAt(&(h->items), heapSize(&h->items) - 1); // remove at last index
    if (heapSize(&h->items) != 0) { // if the heap is not empty after removal
        h->items.array[0] = tempLast;
        minHeapifyDown(h, 0);
    }
    return tempMin;
}

/*
* return the item with the smallest key (== highest priority).
* The item remains in the queue.
*/
Data _min(heap* h) {
    return h->items.array[0];
}

void swap(array* a, int i1, int i2) {
    Data temp = a->array[i1];
    a->array[i1] = a->array[i2];
    a->array[i2] = temp;
}

/*Maintains the heap properties*/
void minHeapifyDown(heap* h, int index)
{
    int l = left(index);
    int r = right(index);
    int smallest;
    if (l <= heapSize(&h->items) - 1 && compareData(h->items.array[l], h->items.array[index]) < 0)
        smallest = l;
    else
        smallest = index;
    if (r <= heapSize(&h->items) - 1 && compareData(h->items.array[r], h->items.array[smallest]) < 0)
        smallest = r;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        minHeapifyDown(h, smallest);
    }
}
/*Maintains the heap properties*/
void minHeapifyUp(heap* h, int index)
{
    int p = parent(index);
    int smallest;
    if (p <= heapSize(&h->items) - 1 && compareData(h->items.array[index], h->items.array[p]) < 0)
        smallest = p;
    else
        smallest = index;
    if (smallest != index)
    {
        swap(&(h->items), index, smallest);
        minHeapifyUp(h, smallest);
    }
}

/*
* PRECONDITION: newKey > item.Key and item already exists in the queue
* POSTCONDITION: x.Key == newKey
* Increases the items priority by assigning it a higher value Key. The properties of the structures must be preserved.
*/
ErrorCode decreaseKey(heap* h, Data item, Data newKey) {
    for (int i = 0; i < heapSize(&h->items); i++) {
        if (compareData(h->items.array[i], item) == 0) {
            if (compareData(h->items.array[i], newKey) == 0) {
                // error new key was larger or equal to the old key
                return NEW_KEY;
            }
            h->items.array[i].key = newKey.key;
            minHeapifyUp(h, i);
            return SUCCESS;
        }
    }
    return H_DATA_EXIST;
}

/*
* Howto: start at index heapSize/2 (all elements after are leafs) go through the non-leafs "backwards" and heapify-down
* builds heap from an unordered list (array)
*/
heap buildMinHeap(Data* unorderedList, size_t size) {
    heap h;
    initHeap(&h, size);
    for (int i = 0; i < size; i++)
        arrayInsert(&h.items, unorderedList[i]);
    for (int j = size / 2; j >= 0; j--)
        minHeapifyDown(&h, j);
    // debug
    /* for (int j = size / 2; j >= 0; j--) {
        minHeapifyDown(&h, j);
        printf("heap\n");
        heapPrintTree(&h);
        printf("array\n");
        (*h.items.printfn)(h.items.used, h.items.array);
        printf("\n");
    } */
    return h;
}

ErrorCode freeHeap(heap* h) {
    if (h != NULL) {
        freeArray(&h->items);
        return SUCCESS;
    } else return FREE_NULLPTR;
}

/////////////////////////////////
// test functions/extra functions


bool testHeapIntegrity(heap* h) {
    for (int i = 0; i < heapSize(&h->items); i++) {
        int l = left(i);
        int r = right(i);
        Data i1 = h->items.array[i];
        if (l < heapSize(&h->items)) {
            Data l1 = h->items.array[l];
            if (compareData(i1, l1) > 0)
            {
                return false;
            }
        }
        if (r < heapSize(&h->items)) {
            Data r1 = h->items.array[r];
            if (compareData(i1, r1) > 0) {
                return false;
            }
        }
    }
    return true;
}


void heapPrintTree(heap* h) {
    int y = 0;
    int x = 0;
    for (int i = 0; i < heapSize(&h->items); i++) {
        for (int j = 0; j < pow(2, i) && j + pow(2, i) <= heapSize(&h->items); j++) {
            x = j + (int)pow(2, i) - 1;
            y = h->items.used;
            if (y > x) {
                printf("[k%f|%c]", h->items.array[x].key, h->items.array[x].element);
            }
            else printf("----");
        }
        printf("\n");
    }
}