/**
 * @file dynamic_array.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief A generic type dynamic array
 * @version 0.1
 * @date 2022-05-31
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
//#include "../utils/error.h"
#include "../datatype.h"

#define QUARTER 0.25

/**
 * The dynamic array stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory. The void* pointer is typedefed to voidp
 * to make it easier to read.
 *
 * The data structure needs a comparison function
 * from the user to be able to function correctly.
 * The function should return 1 when the left value is bigger
 * than the right value, -1 when the opposite is true and
 * 0 when they are equal.
 */

typedef enum
{                               // decrease memory results
    MEM_HALVED = 0,             // memory halved
    MEM_MS_REACHED_NO_NEED = 1, // MEMory Min Size REACHED or no need
    ERRMEM_DECREASE = 2,        // No MEMory DECREASE (error, check errc global)
} MEM;

struct dynamicArray
{
    voidp_t *array;
    size_t size;
    size_t used;
    int (*compare)(cvoidp_t x, cvoidp_t y);
    void (*freeObject)(voidp_t);
};

typedef struct dynamicArray darray;

/**
 * @brief get amount of elements of an array
 *
 * @param a array to check
 * @return element count
 */
size_t da_count(darray *a);
/**
 * @brief get size of an array
 *
 * @param a array to check
 * @return array size
 */
size_t da_size(darray *a);
/**
 * create an empty dynamic array
 */
darray da_create_empty();
/**
 * initialize a dynamic array
 *
 * @param a pointer to the array to initialize
 * @param init_size the initial size of the array
 * @param compare comparison function to compare items
 * @param freeObject function to free an item
 * @return the initial size of the array or 0 if error
 */
size_t da_init(darray *a, size_t init_size, int (*compare)(cvoidp_t x, cvoidp_t y), void (*freeObject)(voidp_t));
/**
 * @brief free the array but not the items
 * 
 * @param a array to free
 * @return true if success
 */
bool da_destroy(darray *a);
/**
 * remove (free) all items in array and then free allocated memory for the dynamic array
 *
 * @param a array to free
 * @return true if success
 */
bool da_free(darray *a);
/**
 * remove (free) all items in the array
 *
 * @param a array to remove from
 * @return the amount of items that was removed or 0 if error
 */
size_t da_clear(darray *a);
/**
 * insert into the dynamic array
 *
 * @param a array to insert into
 * @param item item to insert
 * @return the amount of used indecies in the array or 0 if error
 */
size_t da_insert(darray *a, voidp_t item);
/**
 * remove the last element of the dynamic array
 *
 * @param a array to remove from
 * @return a pointer to the removed item
 */
voidp_t da_remove_last(darray *a);
/**
 * remove a specific item from the dynamic array
 *
 * @param a array to remove from
 * @param item item to remove
 * @return a pointer to the removed item or null if error
 */
voidp_t da_remove_item(darray *a, voidp_t item);
/**
 * remove an item at a specific index from the dynamic array
 *
 * @param a array to remove from
 * @param index index to remove item from
 * @return a pointer to the removed item
 */
voidp_t da_remove_at(darray *a, int index);
/**
 * merge two dynamic arrays, b will be put on the end of a and b will be freed
 *
 * @param a array to merge to (result array)
 * @param b array to merge with (will be freed)
 * @return the new size of the array (a) or 0 if error
 */
size_t da_merge(darray *a, darray *b); // add array b on the end of array a O(n)
/**
 * check if the dynamic array is uninitialized
 *
 * @param a array to try
 * @return true if a.array == NULL
 */
bool da_is_null(darray *a);
/**
 * check if the dynamic array is empty
 *
 * @param a array to try
 * @return true if a.used == 0
 */
bool da_is_empty(darray *a);
/**
 * @brief find an item in the array
 *
 * @param a array to check
 * @param item item to look for
 * @return the index of the item or -1 if it doesnt exist
 */
int da_find(darray *a, voidp_t item);
/**
 * @brief check if an item exists in the array
 *
 * @param a array to check
 * @param item item to look for
 * @return true if exists
 * @return false if not exists
 */
bool da_exists(darray *a, voidp_t item);
#endif