/**
 * @file priority_queue.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief A generic priority queue
 * @version 0.1
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "../s_heap/s_heap.h"
#include "../utils/error.h"

typedef void* voidp;
typedef const void* cvoidp;
typedef unsigned long long size_t;

typedef struct {
    s_heap h;
}PriorityQueue;
/**
 * The priority queue stores pointers to data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory. The void* pointer is typedefed to voidp to
 * make it easier to read.
 * 
 * The data structure needs a comparison function (compare)
 * from the user to be able to function correctly.
 * The function should return 1 when the left value is bigger
 * than the right value, -1 when the opposite is true and
 * 0 when they are equal. It it up to the user to define
 * how you want to sort your items but this is the default way.
 * 
 * The data structure is built upon a min heap (s_heap).
 * To enable testing functions define __TESTING__
 */


/**
 * @brief create an empty priority queue
 * 
 * @return priority queue initialized to 0 and null
 */
PriorityQueue createEmptyPQ();
/**
 * @brief initialize a priority queue
 * 
 * @param pq pointer to the priority queue to initialize
 * @param size the initial size of the priority queue
 * @param compare a function that can compare two items of your data type
 * @return the size the priority queue is initialized to or -1 if error
 */
size_t initPQ(PriorityQueue* pq, size_t size, int (*compare)(cvoidp x, cvoidp y));
/**
 * @brief remove (free) all items in the priority queue and free it
 * 
 * @param pq the priority queue to free
 */
void freePQ(PriorityQueue* pq);

/**
 * @brief get the item count of the priority queue
 * 
 * @param pq pointer to the priority queue to get the count from
 * @return item count
 */
size_t count(PriorityQueue* pq);
/**
 * @brief enqueue an item into the priority queue
 * 
 * @param pq the priority queue to enqueue the item
 * @param item the item to enqueue
 * @return  the item count in the priority queue or -1 if error
 */
size_t enqueuePQ(PriorityQueue* pq, voidp item);
/**
 * @brief dequeue an item from the priority queue
 * 
 * @param pq priority queue to dequeue from
 * @return a pointer to the item that was dequeued
 */
voidp dequeuePQ(PriorityQueue* pq);
/**
 * @brief try to dequeue an item
 * 
 * @param pq the priority queue to try dequeue on
 * @param out a pointer to where the dequeued item will be assigned
 * @return true if there was an item to dequeue
 * @return false if there wasn't an item to dequeue
 */
bool trydequeuePQ(PriorityQueue* pq, voidp* out);
/**
 * @brief peek at the top of the priority queue
 * 
 * @param pq priority queue to peek
 * @param out a pointer to the item at the top of the priority queue
 * @return true if there was an item
 * @return false if there was no item
 */
bool peekPQ(PriorityQueue* pq, voidp* out);

#endif
