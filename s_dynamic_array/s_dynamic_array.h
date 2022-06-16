/**
 * @file dynamic_array.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief Simpler dynamic array, a generic basic functionality array
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/error.h"

#define QUARTER 0.25

/**
 * The s_dynamic array stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory. The void* pointer is typedefed to voidp
 * to make it easier to read.
 *
 * The differences between the s_dynamic array (this) and the dynamic array
 * is functionality. The s_dynamic array has less functionality than the
 * dynamic array, s_dynamic array works more like a basic linked list.
 * There is no way for the data structure to find an item in itself by itself.
 * It blindly inserts data and removes data on by the user specified indecies.
 * Hence the s_dynamic array doesnt need a comparison function (which is the advantage)
 * and only has the main functionality: insert, remove and merge.
 */

typedef void *voidp;
typedef const void *cvoidp;
typedef unsigned long long size_t;

typedef enum
{                               // decrease memory results
    MEM_HALVED = 0,             // memory halved
    MEM_MS_REACHED_NO_NEED = 1, // MEMory Min Size REACHED or no need
    NMEM_DECREASE = 2,          // No MEMory DECREASE (error, check errc global)
} MEM;

typedef struct
{
    voidp *array;
    size_t size;
    size_t used;
} s_dynamicArray;

typedef s_dynamicArray s_array;

/**
 * @brief get amount of elements of an array
 *
 * @param a array to check
 * @return element count
 */
size_t sda_count(s_array *a);
/**
 * @brief get size of an array
 *
 * @param a array to check
 * @return array size
 */
size_t sda_size(s_array *a);
/**
 * create an empty dynamic array
 */
s_dynamicArray sda_create_empty();
/**
 * initialize a dynamic array
 *
 * @param a pointer to the array to initialize
 * @param initSize the initial size of the array
 * @param compare comparison function to compare Data
 * @return the initial size of the array or -1 if error
 */
size_t sda_init(s_dynamicArray *a, size_t initSize);
/**
 * remove (free) all items in the array
 *
 * @param a array to remove from
 * @return the amount of items that was removed or -1 if error
 */
size_t sda_clear(s_array *a);
/**
 * @brief free array (items not freed)
 *
 * @param a array to free
 */
void sda_destroy(s_dynamicArray *a);
/**
 * remove (free) all items in array and then free allocated memory for the dynamic array
 *
 * @param a array to free
 */
void sda_free(s_dynamicArray *a);
/**
 * insert into the dynamic array
 *
 * @param a array to insert into
 * @param item item to insert
 * @return the amount of used indecies in the array or -1 if error
 */
size_t sda_insert(s_dynamicArray *a, voidp item);
/**
 * remove the last element of the dynamic array
 *
 * @param a array to remove from
 * @return a pointer to the removed item
 */
voidp sda_remove_last(s_dynamicArray *a);
/**
 * remove a specific item from the dynamic array
 *
 * @param a array to remove from
 * @param item item to remove
 * @return a pointer to the removed item or null if error
 */
voidp sda_remove_at(s_dynamicArray *a, int index);
/**
 * merge two dynamic arrays, b will be put on the end of a and b will be freed
 *
 * @param a array to merge to (result array)
 * @param b array to merge with (will be freed)
 * @return the new size of the array (a) or -1 if error
 */
size_t sda_merge(s_dynamicArray *a, s_dynamicArray *b);
/**
 * check if the dynamic array is uninitialized
 *
 * @param a array to try
 * @return true if a.array == NULL
 */
bool sda_is_null(s_array *a);
/**
 * check if the dynamic array is empty
 *
 * @param a array to try
 * @return true if a.used == 0
 */
bool sda_is_empty(s_array *a);
/**
 * @brief find an item in the array
 *
 * @param a array to check
 * @param item item to look for
 * @return the index of the item or -1 if it doesnt exist
 */
static MEM sda_memory_decrease(s_dynamicArray *a);
#endif