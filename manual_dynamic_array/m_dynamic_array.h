#ifndef M_DYNAMIC_ARRAY_H
#define M_DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/error.h"

// data type -->
#include "data.h"
// data type <--

// decrease/increase results
typedef enum
{
    MEM_HALVED = 0,     // memory halved
    MEM_MS_REACHED = 1, // MEMory Min Size REACHED
    NMEM_DECREASE = 2,  // No MEMory DECREASE
    NMEM_INCREASE = 3,  // No MEMory INCREASE
    MEM_DOUBLED = 4,    // memory doubled
} MEM;

#define QUARTER 0.25

typedef struct
{
    Data *array;
    size_t size;
    size_t used;
} m_dynamicArray;

typedef m_dynamicArray m_array;

/**
 * create an empty manual dynamic array
 *
 * @return m_dynamicArray
 */
m_dynamicArray m_createEmptyDynamicArray();

/**
 * initialize a manual dynamic array
 *
 * @param a pointer to the array to be initialized
 * @param initSize desired initial size of the array
 * @return size of the manual dynamic array that was created
 */
size_t m_initDynamicArray(m_dynamicArray *a, size_t initSize);

/**
 * free a manual dynamic array
 *
 * @param a array to free
 */
void m_freeArray(m_dynamicArray *a);

/**
 * initializes a manual dynamic array with placeholder items
 *
 * @param a array to initialize
 */
void m_arrayInitialize(m_array *a);

/**
 * insert into manual dynamic array
 *
 * @param a array to insert into
 * @param item item to insert
 * @param index index to insert the item into
 * @return index the item was inserted
 */
int m_arrayInsert(m_dynamicArray *a, Data item, int index);

/**
 * removes an item from the manual dynamic array by inserting a placeholder at the given index
 *
 * @param a array to remove from
 * @param index index to where to remove an item
 * @return index where the item was removed
 */
int m_arrayRemoveAt(m_dynamicArray *a, int index);

/**
 * makes a copy of a manual dynamic array
 *
 * @param copy array to copy from
 * @param out array to copy to
 * @return size of the out array
 */
int m_arrayCopy(m_dynamicArray *copy, m_dynamicArray *out);

/**
 * check if it is appropriate to increase allocated memory
 *
 * @param a array to check
 * @return bool
 */
bool m_checkMemoryIncrease(m_dynamicArray *a);

/**
 * check if it is appropriate to decrease allocated memory
 *
 * @param a array to check
 * @return bool
 */
bool m_checkMemoryDecrease(m_dynamicArray *a);

/**
 * doubles the size of an array
 *
 * @param a array to double
 * @return MEM result of allocation
 */
MEM m_memoryIncrease(m_dynamicArray *a);

/**
 * halves the size of an array
 *
 * @param a array to halve
 * @return MEM result of allocation
 */
MEM m_memoryDecrease(m_dynamicArray *a);

#endif