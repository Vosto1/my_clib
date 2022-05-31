#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include "../dynamic_array/dynamic_array.h"
#include "heap.h"

typedef char Element;
typedef double Key;

typedef struct {
    Key key;
    Element element;
}Item;


typedef unsigned long long size_t;

// user defined functions
int compare(Data x, Data y);
void decrementKey(Item* base, void** out);
Data setKey(Data data, Data key);
void minKey(Data base, Data* out);
void heapPrintTree(heap* h);
#endif