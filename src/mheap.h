/**
 * @file mheap.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief Simple heap (only the minimum of heap functionality), a generic heap built on a dynamic array.
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef M_HEAP_H
#define M_HEAP_H

#include <limits.h>
#include "mdarray.h"
#include "datatype.h"

/**
 * The mheap stores pointers to the data as void* pointers.
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
 * The differences between the mheap (this) and the heap is functionality.
 * The mheap has less functionality than the heap,
 * mheap has minimum main heap functionality: insert, peek, extract
 */

typedef struct
{
    mdarray items;
    int (*compare)(const void* x, const void* y);
} mheap;

/**
 * @brief create an empty mheap
 *
 * @return mheap initialized to zero and null
 */
mheap mh_create_empty();
/**
 * @brief initialize an mheap
 *
 * @param h pointer to the mheap to initialize
 * @param size the initial size of the mheap
 * @param compare a function that can compare two items of your data type
 * @param freeObject a function that can free an item of your data type
 * @return the size the heap is initialized to on success
 */
int mh_init(mheap *h, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*));
/**
 * @brief remove (free) all items in the mheap and free the mheap
 *
 * @param h the mheap to free
 * @return true on success
 */
bool mh_free(mheap *h);
/**
 * @brief free the heap but not the items
 * 
 * @param h heap
 * @return true on success
 * @return false on success
 */
bool mh_destroy(mheap *h);
/**
 * @brief get the size of the mheap (item count)
 *
 * @param h the mheap to get the size of
 * @return item count
 */
uint mh_size(mheap *h);
/**
 * @brief peek the top item in the mheap without removing it
 *
 * @param h mheap to peek
 * @return pointer to the item with the smallest key
 */
void* mh_peek(mheap *h);
/**
 * @brief check if a mheap is null
 *
 * @param h mheap to check
 * @return true if null
 * @return false it not null
 */
bool mh_is_null(mheap *h);
/**
 * @brief check if a mheap is empty (not if its null)
 *
 * @param h mheap to check
 * @return true if empty
 * @return false if not empty
 */
bool mh_is_empty(mheap *h);
/**
 * @brief insert an item of your data type into the mheap
 *
 * @param h mheap to insert into
 * @param item a pointer to the memory of the item
 * @return the item count in the heap on success
 */
int mh_insert(mheap *h, void* item);
/**
 * @brief extracts the top item (with the smallest key) and removes it from the mheap
 *
 * @param h mheap to extract from
 * @return a pointer to the extracted item on success
 */
void* mh_extract_min(mheap *h);
/**
 * @brief build an mheap from an array of your data type (Data)
 *
 * @param unorderedList an array of Data
 * @param size the size of the array of Data
 * @param compare a function that can compare two items of your data type
 * @param freeObject a function that can free an item of your data type
 * @return the heap that was created
 */
mheap mh_build_min_heap(void* *unorderedList, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*));

#ifdef __TESTING__
/**
 * @brief tests the integrity of the mheap
 *
 * @param h mheap to test
 * @return true if test passed
 * @return false if test not passed
 */
bool mh_test_heap_integrity(mheap *h);
#endif

#endif