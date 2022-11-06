#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../datatype.h"

#define QUARTER 0.25 

typedef struct q queue;

struct q {
    void** queue;
    uint front; // the index where the next item to remove is 
    uint end;   // the index where the next inserted item should be at
    uint size;  // size of the memory
    void (*freeObject)(void*);
};

/**
 * @brief create new empty queue
 * 
 * @return queue 
 */
queue q_create_new_empty();
/**
 * @brief initialize a queue
 * 
 * @param q queue to initialize
 * @param size initial size of the queue
 * @return the size of the new queue or 0 if error
 */
size_t q_init(queue *q, size_t size, void (*freeObject)(void*));
/**
 * @brief check if the queue is empty
 * 
 * @param q queue to check
 * @return true if empty
 */
bool q_is_empty(queue* q);
/**
 * @brief enqueue new item
 * 
 * @param q queue to enqueue in
 * @param item item to enqueue
 * @return true if success
 */
bool q_enqueue(queue* q, cvoidp_t item);
/**
 * @brief dequeue the first item
 * 
 * @param q queue to dequeue on
 * @return pointer to dequeued item
 */
voidp_t q_dequeue(queue* q);
/**
 * @brief peek at the item in the front of the queue
 * 
 * @param q queue to peek
 * @return pointer to item in front
 */
cvoidp_t q_peek(queue* q);
/**
 * @brief free all items in the queue and delete the queue 
 * @param queue queue to destroy 
 * @return true if successfull (if unsuccessful heap was NULL pointer)
 */
bool q_destroy(queue* q);
#endif