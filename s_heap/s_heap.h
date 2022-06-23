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
#ifndef _HEAP_H
#define _HEAP_H

#include "../dynamic_array/darray.h"
#include "../utils/error.h"

typedef void *voidp_t;
typedef const void *cvoidp_t;
typedef unsigned long long dim_t;

/**
 * The sheap stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory.The void* pointer is typedefed to voidp
 * to make it easier to read.
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
    darray items;
    int (*compare)(cvoidp_t x, cvoidp_t y);
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
 * @return the size the heap is initialized to or 0 if error
 */
dim_t sh_init(sheap *h, dim_t size, int (*compare)(cvoidp_t x, cvoidp_t y));
/**
 * @brief remove (free) all items in the sheap and free the sheap
 *
 * @param h the sheap to free
 */
void sh_free(sheap *h);

/**
 * @brief get the size of the sheap (item count)
 *
 * @param h the sheap to get the size of
 * @return item count
 */
dim_t sh_size(sheap *h);
/**
 * @brief peek the top item in the sheap without removing it
 *
 * @param h sheap to peek
 * @return pointer to the item with the smallest key
 */
voidp_t sh_peek(sheap *h);
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
 * @return the item count in the heap or a value larger than the item count in the heap if error
 */
dim_t sh_insert(sheap *h, voidp_t item);
/**
 * @brief extracts the top item (with the smallest key) and removes it from the sheap
 *
 * @param h sheap to extract from
 * @return a pointer to the extracted item or null if error
 */
voidp_t sh_extract_min(sheap *h);
/**
 * @brief build an sheap from an array of your data type (Data)
 *
 * @param unorderedList an array of Data
 * @param size the size of the array of Data
 * @param compare a function that can compare two items of your data type
 * @return the heap that was created
 */
sheap sh_build_min_heap(voidp_t *unorderedList, dim_t size, int (*compare)(cvoidp_t x, cvoidp_t y));

#ifdef __TESTING__
/**
 * @brief tests the integrity of the sheap
 *
 * @param h sheap to test
 * @return true if test passed
 * @return false if test not passed
 */
bool s_test_heap_integrity(sheap *h);
#endif

#endif