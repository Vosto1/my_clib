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
#include "../datatype.h"

#define QUARTER 0.25

/**
 * The s_dynamic array stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory. The void* pointer is typedefed to voidp_t
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

typedef enum
{                               // decrease memory results
    MEM_HALVED = 0,             // memory halved
    MEM_MS_REACHED_NO_NEED = 1, // MEMory Min Size REACHED or no need
    ERRMEM_DECREASE = 2,          // ERRor MEMory DECREASE (error, check errc global)
} MEM;

struct s_dynamicArray
{
    voidp_t *array;
    dim_t size;
    dim_t used;
};

typedef struct s_dynamicArray sdarray;

/**
 * @brief get amount of elements of an array
 *
 * @param a array to check
 * @return element count
 */
dim_t sda_count(sdarray *a);
/**
 * @brief get size of an array
 *
 * @param a array to check
 * @return array size
 */
dim_t sda_size(sdarray *a);
/**
 * create an empty dynamic array
 */
sdarray sda_create_empty();
/**
 * initialize a dynamic array
 *
 * @param a pointer to the array to initialize
 * @param init_size the initial size of the array
 * @return the size of the new array or 0 if error
 */
dim_t sda_init(sdarray *a, dim_t init_size);
/**
 * remove (free) all items in the array
 *
 * @param a array to remove from
 * @return the amount of items that was removed or 0 if error
 */
dim_t sda_clear(sdarray *a);
/**
 * @brief free array (items not freed)
 *
 * @param a array to free
 */
void sda_destroy(sdarray *a);
/**
 * remove (free) all items in array and then free allocated memory for the dynamic array
 *
 * @param a array to free
 */
void sda_free(sdarray *a);
/**
 * @brief get item at index
 * 
 * @param a array to find item at index
 * @return pointer to item at index or NULL if index out of bounds
 */
cvoidp_t sda_at(sdarray * a, dim_t index);
/**
 * insert into the dynamic array
 *
 * @param a array to insert into
 * @param item item to insert
 * @return the amount of used indecies in the array or 0 if error
 */
dim_t sda_insert(sdarray *a, voidp_t item);
/**
 * remove the last element of the dynamic array
 *
 * @param a array to remove from
 * @return a pointer to the removed item
 */
voidp_t sda_remove_last(sdarray *a);
/**
 * remove a specific item from the dynamic array
 *
 * @param a array to remove from
 * @param item item to remove
 * @return a pointer to the removed item or null if error
 */
voidp_t sda_remove_at(sdarray *a, int index);
/**
 * merge two dynamic arrays, b will be put on the end of a and b will be freed
 *
 * @param a array to merge to (result array)
 * @param b array to merge with (will be freed)
 * @return the new size of the array (a) or 0 if error
 */
dim_t sda_merge(sdarray *a, sdarray *b);
/**
 * check if the dynamic array is uninitialized
 *
 * @param a array to try
 * @return true if a.array == NULL
 */
bool sda_is_null(sdarray *a);
/**
 * check if the dynamic array is empty
 *
 * @param a array to try
 * @return true if a.used == 0
 */
bool sda_is_empty(sdarray *a);
#endif