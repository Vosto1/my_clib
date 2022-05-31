#ifndef _HEAP_H
#define _HEAP_H

#include "../dynamic_array/dynamic_array.h"
#include "../utils/error.h"


typedef void* Data;
typedef unsigned long long size_t;

typedef struct {
    array items;
    int (*compare)(Data x, Data y);
}heap;

heap s_createEmptyHeap();
size_t s_initHeap(heap* h, size_t size, int (*compare)(Data x, Data y));
void s_freeHeap(heap* h);

size_t s_heapSize(heap* h);
Data s_min(heap* h);
bool sh_is_null(heap* h);
bool sh_is_empty(heap* h);

size_t s_heapInsert(heap* h, Data item);
Data s_extractMin(heap* h);
heap s_buildMinHeap(Data* unorderedList, size_t size, int (*compare)(Data x, Data y));

bool s_testHeapIntegrity(heap* h);
int s_parent(int i);
int s_left(int i);
int s_right(int i);
void s_swap(array* a, int i1, int i2);
void s_minHeapifyDown(heap* h, int index);
void s_minHeapifyUp(heap* h, int index);

#endif