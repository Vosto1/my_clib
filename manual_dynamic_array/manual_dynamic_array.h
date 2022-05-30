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
    NMEM_INCREASE = 3, // No MEMory INCREASE
    MEM_DOUBLED = 4, // memory doubled
}MEM;

#define QUARTER 0.25

typedef struct {
    Data* array;
    size_t size;
    size_t used;
}mdynamicArray;

typedef mdynamicArray marray;

/**
 * create an empty manual dynamic array
 * 
 * @return mdynamicArray
 */
mdynamicArray mcreateEmptyDynamicArray();

/**
 * initialize a manual dynamic array
 * 
 * @param a pointer to the array to be initialized
 * @param initSize desired initial size of the array
 * @return size of the manual dynamic array that was created
 */
size_t minitDynamicArray(mdynamicArray* a, size_t initSize);

/**
 * free a manual dynamic array
 * 
 * @param a array to free
 */
void mfreeArray(mdynamicArray* a);

/**
 * initializes a manual dynamic array with placeholder items
 * 
 * @param a array to initialize
 */
void marrayInitialize(marray *a);

/**
 * insert into manual dynamic array
 * 
 * @param a array to insert into
 * @param item item to insert
 * @param index index to insert the item into
 * @return index the item was inserted
 */
int marrayInsert(mdynamicArray* a, Data item, int index);

/**
 * removes an item from the manual dynamic array by inserting a placeholder at the given index
 * 
 * @param a array to remove from
 * @param index index to where to remove an item
 * @return index where the item was removed
 */
int marrayRemoveAt(mdynamicArray* a, int index);

/**
 * makes a copy of a manual dynamic array
 * 
 * @param copy array to copy from
 * @param out array to copy to
 * @return size of the out array
 */
int marrayCopy(mdynamicArray* copy, mdynamicArray* out);

/**
 * check if it is appropriate to increase allocated memory
 * 
 * @param a array to check
 * @return bool
 */
bool mcheckMemoryIncrease(mdynamicArray* a);

/**
 * check if it is appropriate to decrease allocated memory
 * 
 * @param a array to check
 * @return bool
 */
bool mcheckMemoryDecrease(mdynamicArray* a);

/**
 * doubles the size of an array
 * 
 * @param a array to double
 * @return MEM result of allocation
 */
MEM mmemoryIncrease(mdynamicArray* a);

/**
 * halves the size of an array
 * 
 * @param a array to halve
 * @return MEM result of allocation
 */
MEM mmemoryDecrease(mdynamicArray* a);


#endif