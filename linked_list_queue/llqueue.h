#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../datatype.h"
#include "../linked_list/list.h"

typedef ll llqueue;
/**
 * @brief create new empty queue
 * 
 * @return queue 
 */
llqueue llq_create_new_empty();
/**
 * @brief check if the queue is empty
 * 
 * @param q queue to check
 * @return true if empty
 */
bool llq_is_empty(llqueue* q);
/**
 * @brief enqueue new item
 * 
 * @param q queue to enqueue in
 * @param item item to enqueue
 */
void llq_enqueue(llqueue* q, const void* item);
/**
 * @brief dequeue the first item
 * 
 * @param q queue to dequeue on
 * @return pointer to dequeued item
 */
const void* llq_dequeue(llqueue* q);
/**
 * @brief peek at the item in the front of the queue
 * 
 * @param q queue to peek
 * @return pointer to item in front
 */
const void* llq_peek(llqueue* q);
/**
 * @brief get the element count in the queue
 * 
 * @param q queue to get count of
 * @return element count
 */
size_t llq_count(llqueue* q);

#endif