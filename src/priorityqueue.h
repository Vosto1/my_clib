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

#include "mheap.h"
#include "datatype.h"

typedef struct // needs fixing (remove struct and only typedef sheap to pq)
{
    mheap h;
} priorityqueue;
/**
 * The priority queue stores pointers to data as void* pointers.
 * This way the type is generic, but all data must be allocated as
 * dynamic memory.
 *
 * The data structure needs a comparison function (compare)
 * from the user to be able to function correctly.
 * The function should return 1 when the left value is bigger
 * than the right value, -1 when the opposite is true and
 * 0 when they are equal. It it up to the user to define
 * how you want to sort your items but this is the default way.
 *
 * The data structure is built upon a min heap (sheap).
 * To enable testing functions define __TESTING__
 */

/**
 * @brief create an empty priority queue
 *
 * @return priority queue initialized to 0 and null
 */
priorityqueue pq_create_empty();
/**
 * @brief initialize a priority queue
 *
 * @param pq pointer to the priority queue to initialize
 * @param size the initial size of the priority queue
 * @param compare a function that can compare two items of your data type
 * @return the size the priority queue is initialized to on success
 */
int pq_init(priorityqueue *pq, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*));
/**
 * @brief remove (free) all items in the priority queue and free it
 *
 * @param pq the priority queue to free
 * @return true on success
 */
bool pq_free(priorityqueue *pq);
/**
 * @brief free the priority queue but not it's items
 * 
 * @param pq priority queue to destroy
 * @return true on success
 * @return false 
 */
bool pq_destroy(priorityqueue *pq);
/**
 * @brief get the item count of the priority queue
 *
 * @param pq pointer to the priority queue to get the count from
 * @return item count
 */
int pq_count(priorityqueue *pq);
/**
 * @brief enqueue an item into the priority queue
 *
 * @param pq the priority queue to enqueue the item
 * @param item the item to enqueue
 * @return  the item count in the priority queue on success
 */
int pq_enqueue(priorityqueue *pq, void* item);
/**
 * @brief dequeue an item from the priority queue
 *
 * @param pq priority queue to dequeue from
 * @return a pointer to the item that was dequeued
 */
void* pq_dequeue(priorityqueue *pq);
/**
 * @brief try to dequeue an item
 *
 * @param pq the priority queue to try dequeue on
 * @param out a pointer to where the dequeued item will be assigned
 * @return true if there was an item to dequeue
 * @return false if there wasn't an item to dequeue
 */
bool pq_trydequeue(priorityqueue *pq, void* *out);
/**
 * @brief peek at the top of the priority queue
 *
 * @param pq priority queue to peek
 * @param out a pointer to the item at the top of the priority queue
 * @return true if there was an item
 * @return false if there was no item
 */
bool pq_peek(priorityqueue *pq, void* *out);

#endif
