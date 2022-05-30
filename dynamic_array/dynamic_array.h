#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/error.h"

// data type -->
#include "data.h"
// data type <--


// decrease/increase results
typedef enum {
    MEM_HALVED = 0, // memory halved
    MEM_MS_REACHED = 1, // MEMory Min Size REACHED
    NMEM_DECREASE = 2, // No MEMory DECREASE
}MEM;

#define QUARTER 0.25

typedef struct dyn_arr {
    Data* array;
    size_t size;
    size_t used;
    int (*compare)(void* a, void* b);
}dynamicArray;

typedef dynamicArray array;


dynamicArray createEmptyDynamicArray();
size_t initDynamicArray(dynamicArray* a, size_t initSize);
void freeArray(dynamicArray* a);

size_t arrayInsert(dynamicArray* a, Data item);
size_t arrayRemoveLast(dynamicArray* a);
size_t arrayRemoveItem(dynamicArray* a, Data item);
int arrayRemoveAt(dynamicArray* a, int index);
MEM memoryDecrease(dynamicArray* a);

// convert a static array to a dynamic array
size_t convert(Data b[], dynamicArray* a, size_t size);
size_t arrayUnion(dynamicArray* a, dynamicArray* b); // add array b on the end of array a O(n)
size_t arrayClear(array* a);

// framtida funktioner
ErrorCode find(Data item, int* index); // return index (by ref) error if not exists
bool exists(Data item); // returns true if item exists in the array


#endif