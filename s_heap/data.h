#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include "../dynamic_array/dynamic_array.h"
#include "s_heap.h"

typedef char Element;
typedef double Key;

typedef struct {
    Key key;
    Element element;
}Item;


typedef unsigned long long size_t;

// user defined functions
int compare(Data x, Data y);
void heapPrintTree(heap* h);
#endif