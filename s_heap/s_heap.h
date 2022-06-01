/**
 * @file s_heap.h
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

#include "../dynamic_array/dynamic_array.h"
#include "../utils/error.h"


typedef void* Data;
typedef unsigned long long size_t;

/**
 * The s_heap stores pointers to the data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory.The void* pointer is typedefed to Data
 * to make it easier to read.
 * 
 * To enable testing functions define __TESTING__
 * 
 * The data structure needs a Data to Data comparison function
 * (compare) from the user to be able to function correctly.
 * The function should return 1 when the left value is bigger
 * than the right value, -1 when the opposite is true and
 * 0 when they are equal. It it up to the user to define
 * how you want to sort your items but this is the default way.
 * 
 * The differences between the s_heap (this) and the heap is functionality.
 * The s_heap has less functionality than the heap,
 * s_heap has minimum heap functionality: insert, peek 
 */

typedef struct {
    array items;
    int (*compare)(Data x, Data y);
}s_heap;

/**
 * @brief create an empty s_heap
 * 
 * @return s_heap initialized to zero and null
 */
s_heap s_createEmptyHeap();
/**
 * @brief initialize an s_heap
 * 
 * @param h pointer to the s_heap to initialize
 * @param size the initial size of the s_heap
 * @param compare a function that can compare two items of your data type
 * @return the size the heap is initialized to or -1 if something went wrong
 */
size_t s_initHeap(s_heap* h, size_t size, int (*compare)(Data x, Data y));
/**
 * @brief remove (free) all items in the s_heap and free the s_heap
 * 
 * @param h the s_heap to free
 */
void s_freeHeap(s_heap* h);

/**
 * @brief get the size of the s_heap (item count)
 * 
 * @param h the s_heap to get the size of
 * @return item count
 */
size_t s_heapSize(s_heap* h);
/**
 * @brief peek the top item in the s_heap without removing it
 * 
 * @param h s_heap to peek
 * @return pointer to the item with the smallest key
 */
Data s_peek(s_heap* h);
/**
 * @brief check if a s_heap is null
 * 
 * @param h s_heap to check
 * @return true if null
 * @return false it not null
 */
bool sh_is_null(s_heap* h);
/**
 * @brief check if a s_heap is empty (not if its null)
 * 
 * @param h s_heap to check
 * @return true if empty
 * @return false if not empty
 */
bool sh_is_empty(s_heap* h);

/**
 * @brief insert an item of your data type into the s_heap
 * 
 * @param h s_heap to insert into
 * @param item a pointer to the memory of the item
 * @return the item count in the heap or -1 if something went wrong
 */
size_t s_heapInsert(s_heap* h, Data item);
/**
 * @brief extracts the top item (with the smallest key) and removes it from the s_heap
 * 
 * @param h s_heap to extract from
 * @return a pointer to the extracted item or null if something went wrong
 */
Data s_extractMin(s_heap* h);
/**
 * @brief build an s_heap from an array of your data type (Data)
 * 
 * @param unorderedList an array of Data
 * @param size the size of the array of Data
 * @param compare a function that can compare two items of your data type
 * @return the heap that was created
 */
s_heap s_buildMinHeap(Data* unorderedList, size_t size, int (*compare)(Data x, Data y));

#ifdef __TESTING__
/**
 * @brief tests the integrity of the s_heap
 * 
 * @param h s_heap to test
 * @return true if test passed
 * @return false if test not passed
 */
bool s_testHeapIntegrity(s_heap* h);
#endif
/**
 * @brief get the parent index of another index
 * 
 * @param i index to get parent off of
 * @return parent index
 */
static int s_parent(int i);
/**
 * @brief get the left index of another index
 * 
 * @param i index to get left off of
 * @return left index
 */
static int s_left(int i);
/**
 * @brief get the right index of another index
 * 
 * @param i index to get right off of
 * @return right index
 */
static int s_right(int i);
/**
 * @brief swap the item on index i1 with the item on index i2 
 * 
 * @param a the array to swap items in
 * @param i1 index of item 1
 * @param i2 index of item 2
 */
static void s_swap(array* a, int i1, int i2);
/**
 * @brief compare parent with children, if there is a heap violation then switch parent and child then recursively continue down the s_heap
 * 
 * @param h s_heap to heapify
 * @param index index to start on
 */
static void s_minHeapifyDown(s_heap* h, int index);
/**
 * @brief compare child with parent, if there is a heap violation then swap the child and the parent and then recursively continue up the s_heap
 * 
 * @param h s_heap to heapify
 * @param index index to start on
 */
static void s_minHeapifyUp(s_heap* h, int index);

#endif