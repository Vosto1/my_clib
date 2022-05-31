#ifndef _HEAP_H
#define _HEAP_H

#include "dynamic_array.h"
#include "../utils/error.h"

// data type -->
#include "data.h"
// data type <--


typedef struct h {
    array items;
}heap;


size_t heapSize(array* a);
void heapPrint(array* a);
heap createEmptyHeap();
ErrorCode initHeap(heap* h, size_t size);
ErrorCode freeHeap(heap* h);
int parent(int i);
int left(int i);
int right(int i);
ErrorCode heapInsert(heap* h, Data item);
Key minKey(heap* h);
bool heapRemove(heap* h, Data item);
Data extractMin(heap* h);
Data _min(heap* h);
void swap(array* a, int i1, int i2);
void minHeapifyDown(heap* h, int index);
void minHeapifyUp(heap* h, int index);
ErrorCode decreaseKey(heap* h, Data item, Data newKey);
heap buildMinHeap(Data* unorderedList, size_t size);
bool testHeapIntegrity(heap* h);
void heapPrintTree(heap* h);

#endif