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

typedef void* Data;
typedef unsigned long long size_t;

/**
 * The heap stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory. The void* pointer is typedefed to Data to
 * make it easier to read.
 * 
 * To enable testing functions define __TESTING__
 * 
 * The data structure needs a couple of functions defined
 * by the user to function correctly.
 * 
 * - Data to Data comparison (compare)
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

typedef struct {
    array items;
    int (*compare)(Data x, Data y);
    Data (*setKey)(Data x, Data key);
    void (*minKey)(Data base, Data* out);
}heap;

/**
 * @brief create an empty heap
 * 
 * @return heap initialized to zero and null
 */
heap createEmptyHeap();
/**
 * @brief initialize a heap
 * 
 * @param h pointer to the heap to initialize
 * @param size the initial size of the heap
 * @param compare a function that can compare two items of your data type
 * @param setKey a function that can set a key of an item of your data type
 * @param minKey a function that based on a base element of your data type can create a smaller key
 * @return the size the heap is initialized to or -1 if something went wrong
 */
size_t initHeap(heap* h,
                size_t size,
                int (*compare)(Data x, Data y),
                Data (*setKey)(Data x, Data key),
                void (*minKey)(Data base, Data* out));
/**
 * @brief remove (free) all items in the heap and free the heap
 * 
 * @param h the heap to free
 */
void freeHeap(heap* h);

/**
 * @brief get the size of the heap (item count)
 * 
 * @param h the heap to get the size of
 * @return item count
 */
size_t heapSize(heap* h);
/**
 * @brief peek the top item in the heap without removing it
 * 
 * @param h heap to peek
 * @return pointer to the item with the smallest key
 */
Data _min(heap* h);
/**
 * @brief check if a heap is null
 * 
 * @param h heap to check
 * @return true if null
 * @return false it not null
 */
bool h_is_null(heap* h);
/**
 * @brief check if a heap is empty (not if its null)
 * 
 * @param h heap to check
 * @return true if empty
 * @return false if not empty
 */
bool h_is_empty(heap* h);

/**
 * @brief insert an item of your data type into the heap
 * 
 * @param h heap to insert into
 * @param item a pointer to the memory of the item
 * @return the item count in the heap or -1 if something went wrong
 */
size_t heapInsert(heap* h, Data item);
/**
 * @brief removes the specified item from the heap
 * 
 * @param h heap to remove item from
 * @param item a pointer to the item which should be removed
 * @return a pointer to the item removed from the heap or null if something went wrong
 */
Data heapRemove(heap* h, Data item);
/**
 * @brief extracts the top item (with the smallest key) and removes it from the heap
 * 
 * @param h heap to extract from
 * @return a pointer to the extracted item or null if something went wrong
 */
Data extractMin(heap* h);
/**
 * @brief decreases a key of the specified item to the specified key. If the item doesnt exist or th key is equal or larger than the current key an error is set.
 * 
 * @param h heap the item exists in
 * @param item the item to decrease key on
 * @param newKey the new key the item should have
 * @return the new index of the item or -1 if something went wrong
 */
int decreaseKey(heap* h, Data item, Data newKey);
/**
 * @brief build a heap from an array of your data type (Data)
 * 
 * @param unorderedList an array of Data
 * @param size the size of the array of Data
 * @param compare a function that can compare two items of your data type
 * @param setKey a function that can set a key of an item of your data type
 * @param minKey a function that based on a base element of your data type can create a smaller key
 * @return the heap that was created
 */
heap buildMinHeap(Data* unorderedList,
                size_t size,
                int (*compare)(Data x, Data y),
                Data (*setKey)(Data x, Data key),
                void (*minKey)(Data base, Data* out));

#ifdef __TESTING__
/**
 * @brief tests the integrity of the heap
 * 
 * @param h heap to test
 * @return true if test passed
 * @return false if test not passed
 */
bool testHeapIntegrity(heap* h);
#endif
/**
 * @brief get the parent index of another index
 * 
 * @param i index to get parent off of
 * @return parent index
 */
static int parent(int i);
/**
 * @brief get the left index of another index
 * 
 * @param i index to get left off of
 * @return left index
 */
static int left(int i);
/**
 * @brief get the right index of another index
 * 
 * @param i index to get right off of
 * @return right index
 */
static int right(int i);
/**
 * @brief swap the item on index i1 with the item on index i2 
 * 
 * @param a the array to swap items in
 * @param i1 index of item 1
 * @param i2 index of item 2
 */
static void swap(array* a, int i1, int i2);
/**
 * @brief compare parent with children, if there is a heap violation then switch parent and child then recursively continue down the heap
 * 
 * @param h heap to heapify
 * @param index index to start on
 */
static void minHeapifyDown(heap* h, int index);
/**
 * @brief compare child with parent, if there is a heap violation then swap the child and the parent and then recursively continue up the heap
 * 
 * @param h heap to heapify
 * @param index index to start on
 */
static void minHeapifyUp(heap* h, int index);

#endif