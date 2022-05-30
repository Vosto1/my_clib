#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "stdlib.h"
#include "math.h"
#include "../utils/error.h"

// data type -->
#include "data.h"
// data type <--


#define QUARTER 0.25

typedef struct dyn_arr {
    Data* array;
    size_t size;
    size_t used;
}dynamicArray;

typedef dynamicArray array;


dynamicArray createEmptyDynamicArray();
ErrorCode initDynamicArray(dynamicArray* a, size_t initSize);
ErrorCode freeArray(dynamicArray* a);

ErrorCode arrayInsert(dynamicArray* a, Data item);
ErrorCode arrayRemoveLast(dynamicArray* a);
ErrorCode arrayRemoveItem(dynamicArray* a, Data item);
ErrorCode arrayRemoveAt(dynamicArray* a, int index);
ErrorCode memoryDecrease(dynamicArray* a);

// convert a static array to a dynamic array
ErrorCode convert(Data b[], dynamicArray* a, size_t size);
ErrorCode arrayUnion(dynamicArray* a, dynamicArray* b); // add array b on the end of array a O(n)
ErrorCode arrayClear(array *a);

// framtida funktioner
ErrorCode find(Data item, int* index); // return index (by ref) error if not exists
bool exists(Data item); // returns true if item exists in the array


#endif