/**
 * @file heap.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief A generic heap built on a dynamic array
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef _HEAP_H
#define _HEAP_H

#include "../dynamic_array/dynamic_array.h"
#include "../utils/error.h"

typedef void *voidp_t;
typedef const void *cvoidp_t;
typedef unsigned long long size_t;

/**
 * The heap stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory. The void* pointer is typedefed to voidp to
 * make it easier to read.
 *
 * To enable testing functions define __TESTING__
 *
 * The data structure needs a couple of functions defined
 * by the user to function correctly.
 *
 * - comparison (compare)
 * It should return 1 when the left value is bigger
 * than the right value, -1 when the opposite is true and
 * 0 when they are equal. It it up to the user to define
 * how you want to sort your items but this is the default way.
 *
 * - update key of an element (setKey)
 * It should take an in variable "key" and an out variable "item"
 * ("return" by reference). The key of the out variable "item" should
 * be set to the key of the in variable "key".
 *
 * - create a smallest key (minKey)
 * It should take an in variable "base" and an out variable "out".
 * From the base (which will be the currently smallest key in the heap)
 * the function should calculate an guaranteed smallest key in the heap
 * (as long as the heap is not modified after).
 * Example: new key = base.key - 1. That will be the smallest
 * new key. The key should then be assigned to the out variable "out".
 */

typedef struct
{
    darray items;
    int (*compare)(cvoidp_t x, cvoidp_t y);
    voidp_t (*setKey)(voidp_t x, voidp_t key);
    void (*minKey)(voidp_t base, voidp_t *out);
} heap;

/**
 * @brief create an empty heap
 *
 * @return heap initialized to zero and null
 */
heap h_create_empty();
/**
 * @brief initialize a heap
 *
 * @param h pointer to the heap to initialize
 * @param size the initial size of the heap
 * @param compare a function that can compare two items of your data type
 * @param setKey a function that can set a key of an item of your data type
 * @param minKey a function that based on a base element of your data type can create a smaller key
 * @return the size the heap is initialized to or 0 if error
 */
size_t h_init(heap *h,
                size_t size,
                int (*compare)(cvoidp_t x, cvoidp_t y),
                voidp_t (*setKey)(voidp_t x, voidp_t key),
                void (*minKey)(voidp_t base, voidp_t *out));
/**
 * @brief remove (free) all items in the heap and free the heap
 *
 * @param h the heap to free
 */
void h_free(heap *h);

/**
 * @brief get the size of the heap (item count)
 *
 * @param h the heap to get the size of
 * @return item count
 */
size_t h_size(heap *h);
/**
 * @brief peek the top item in the heap without removing it
 *
 * @param h heap to peek
 * @return pointer to the item with the smallest key
 */
voidp_t h_min(heap *h);
/**
 * @brief check if a heap is null
 *
 * @param h heap to check
 * @return true if null
 * @return false it not null
 */
bool h_is_null(heap *h);
/**
 * @brief check if a heap is empty (not if its null)
 *
 * @param h heap to check
 * @return true if empty
 * @return false if not empty
 */
bool h_is_empty(heap *h);

/**
 * @brief insert an item of your data type into the heap
 *
 * @param h heap to insert into
 * @param item a pointer to the memory of the item
 * @return the item count in the heap or a value larger than the size of the heap is error
 */
size_t h_insert(heap *h, voidp_t item);
/**
 * @brief removes the specified item from the heap
 *
 * @param h heap to remove item from
 * @param item a pointer to the item which should be removed
 * @return a pointer to the item removed from the heap or null if error
 */
voidp_t h_remove(heap *h, voidp_t item);
/**
 * @brief extracts the top item (with the smallest key) and removes it from the heap
 *
 * @param h heap to extract from
 * @return a pointer to the extracted item or null if error
 */
voidp_t h_extract_min(heap *h);
/**
 * @brief decreases a key of the specified item to the specified key. If the item doesnt exist or th key is equal or larger than the current key an error is set.
 *
 * @param h heap the item exists in
 * @param item the item to decrease key on
 * @param newKey the new key the item should have
 * @return the new index of the item or a value larger than the size of the heap is error
 */
size_t h_decrease_key(heap *h, voidp_t item, voidp_t newKey);
/**
 * @brief build a heap from an array
 *
 * @param unorderedList an array of pointers to elements
 * @param size the size of the array
 * @param compare a function that can compare two items of your data type
 * @param setKey a function that can set a key of an item of your data type
 * @param minKey a function that based on a base element of your data type can create a smaller key
 * @return the heap that was created
 */
heap h_build_min_heap(voidp_t *unorderedList,
                  size_t size,
                  int (*compare)(cvoidp_t x, cvoidp_t y),
                  voidp_t (*setKey)(voidp_t x, voidp_t key),
                  void (*minKey)(voidp_t base, voidp_t *out));
#ifdef __TESTING__
/**
 * @brief tests the integrity of the heap
 *
 * @param h heap to test
 * @return true if test passed
 * @return false if test not passed
 */
bool test_heap_integrity(heap *h);
#endif

#endif