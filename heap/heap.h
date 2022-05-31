#ifndef _HEAP_H
#define _HEAP_H

#include "../dynamic_array/dynamic_array.h"
#include "../utils/error.h"

typedef void* Data;
typedef unsigned long long size_t;

typedef struct {
    array items;
    int (*compare)(Data x, Data y);
    Data (*setKey)(Data x, Data key);
    void (*minKey)(Data base, Data* out);
}heap;

heap createEmptyHeap();
size_t initHeap(heap* h,
                size_t size,
                int (*compare)(Data x, Data y),
                Data (*setKey)(Data x, Data key),
                void (*minKey)(Data base, Data* out));
void freeHeap(heap* h);

size_t heapSize(heap* h);
Data _min(heap* h);
bool h_is_null(heap* h);
bool h_is_empty(heap* h);

size_t heapInsert(heap* h, Data item);
Data heapRemove(heap* h, Data item);
Data extractMin(heap* h);
int decreaseKey(heap* h, Data item, Data newKey);
heap buildMinHeap(Data* unorderedList,
                size_t size,
                int (*compare)(Data x, Data y),
                Data (*setKey)(Data x, Data key),
                void (*minKey)(Data base, Data* out));

bool testHeapIntegrity(heap* h);
int parent(int i);
int left(int i);
int right(int i);
void swap(array* a, int i1, int i2);
void minHeapifyDown(heap* h, int index);
void minHeapifyUp(heap* h, int index);

#endif