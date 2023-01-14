/**
 * @file sheap.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief Simple heap (only the minimum of heap functionality), a generic heap built on a dynamic array.
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef S_HEAP_H
#define S_HEAP_H

#include <limits.h>
#include "../s_dynamic_array/sdarray.h"
#include "../datatype.h"

/**
 * The sheap stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory.
 *
 * To enable testing functions define __TESTING__
 *
 * The data structure needs a comparison function
 * (compare) from the user to be able to function correctly.
 * The function should return 1 when the left value is bigger
 * than the right value, -1 when the opposite is true and
 * 0 when they are equal. It it up to the user to define
 * how you want to sort your items but this is the default way.
 *
 * The differences between the sheap (this) and the heap is functionality.
 * The sheap has less functionality than the heap,
 * sheap has minimum main heap functionality: insert, peek, extract
 */

typedef struct
{
    sdarray items;
    int (*compare)(const void* x, const void* y);
} sheap;

/**
 * @brief create an empty sheap
 *
 * @return sheap initialized to zero and null
 */
sheap sh_create_empty();
/**
 * @brief initialize an sheap
 *
 * @param h pointer to the sheap to initialize
 * @param size the initial size of the sheap
 * @param compare a function that can compare two items of your data type
 * @param freeObject a function that can free an item of your data type
 * @return the size the heap is initialized to on success
 */
int sh_init(sheap *h, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*));
/**
 * @brief remove (free) all items in the sheap and free the sheap
 *
 * @param h the sheap to free
 * @return true on success
 */
bool sh_free(sheap *h);
/**
 * @brief free the heap but not the items
 * 
 * @param h heap
 * @return true on success
 * @return false on success
 */
bool sh_destroy(sheap *h);
/**
 * @brief get the size of the sheap (item count)
 *
 * @param h the sheap to get the size of
 * @return item count
 */
uint sh_size(sheap *h);
/**
 * @brief peek the top item in the sheap without removing it
 *
 * @param h sheap to peek
 * @return pointer to the item with the smallest key
 */
void* sh_peek(sheap *h);
/**
 * @brief check if a sheap is null
 *
 * @param h sheap to check
 * @return true if null
 * @return false it not null
 */
bool sh_is_null(sheap *h);
/**
 * @brief check if a sheap is empty (not if its null)
 *
 * @param h sheap to check
 * @return true if empty
 * @return false if not empty
 */
bool sh_is_empty(sheap *h);
/**
 * @brief insert an item of your data type into the sheap
 *
 * @param h sheap to insert into
 * @param item a pointer to the memory of the item
 * @return the item count in the heap on success
 */
int sh_insert(sheap *h, void* item);
/**
 * @brief extracts the top item (with the smallest key) and removes it from the sheap
 *
 * @param h sheap to extract from
 * @return a pointer to the extracted item on success
 */
void* sh_extract_min(sheap *h);
/**
 * @brief build an sheap from an array of your data type (Data)
 *
 * @param unorderedList an array of Data
 * @param size the size of the array of Data
 * @param compare a function that can compare two items of your data type
 * @param freeObject a function that can free an item of your data type
 * @return the heap that was created
 */
sheap sh_build_min_heap(void* *unorderedList, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*));

#ifdef __TESTING__
/**
 * @brief tests the integrity of the sheap
 *
 * @param h sheap to test
 * @return true if test passed
 * @return false if test not passed
 */
bool sh_test_heap_integrity(sheap *h);
#endif

#endif