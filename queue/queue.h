#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../datatype.h"
//#include "../hash_dictionary/hashtable.h"

/* 1
    I need to find a good way to move all items in the queue when needed (it's not possible to use sdarray this time)
    Maybe I should create a new array implementation that is circular, and only expands when it is needed.

    things to note: 
    1. when the first_element and last_element point to items beside eachother the queue is full and needs to be resized
    2. I cant resize the queue without reordering all the items.
        2.1 I need to retain the old size after a resize so that I can reorder the items correctly i.e. with modulu
            (the elements might have overflowed to the begining of the array, and I need to use modulu with the old array size to reorder them in the new array size)
        2.2 Inplace reordering might be possible if I use the count of elements and begin backwards
            - Almost inplace reordering:
                1. What I can do is to copy from the back of the array to a new temporary array as many items that have overflowed to the front of the array
                   (in order to make space at the back for the overflowed items in the front of the array)
                2. Start copying from the back of the queue into the new resized array spaces, find the last index by getting the count of elements minus 1.
                3. this should make sure that I can copy from the back of the queue to the front with as little temporary arrays (as in size) as possible
*/

/* 2
    Or I can have rules where the queue keeps track of where in the array the indecies end up when they wrap around and then the array needs to be resized
    - Example:
                On enqueue...
                (1 enqueue per row)
                                            indecies:0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21
                 array: [ : : : : :1:2:3:4:5:6] --> [7:  :  :  :  : 1: 2: 3: 4: 5: 6] |  |  |  |  |  |  |  |  |  |  |    // rules:  x
                                                    [7: 8:  :  :  : 1: 2: 3: 4: 5: 6] |  |  |  |  |  |  |  |  |  |  |    // rules:  x
                                                    [7: 8: 9:  :  : 1: 2: 3: 4: 5: 6] |  |  |  |  |  |  |  |  |  |  |    // rules:  x
                                                    [7: 8: 9: 1:  : 1: 2: 3: 4: 5: 6] |  |  |  |  |  |  |  |  |  |  |    // rules:  x
                                                    [7: 8: 9: 1: 2: 1: 2: 3: 4: 5: 6] |  |  |  |  |  |  |  |  |  |  |    // rules:  x
                                                    Overflow on enqueue 3! Doubling size of array...
                                                    [7: 8: 9: 1: 2: 1: 2: 3: 4: 5: 6: 3:  :  :  :  :  :  :  :  :  : x]   // rules:  index 4s' previous index is 11
                                                    [7: 8: 9: 1: 2: 1: 2: 3: 4: 5: 6: 3: 4:  :  :  :  :  :  :  :  : x]   // rules:  index 4s' previous index is 11
                                                    [7: 8: 9: 1: 2: 1: 2: 3: 4: 5: 6: 3: 4: 5:  :  :  :  :  :  :  : x]   // rules:  index 4s' previous index is 11
                                                    etc...
                On dequeue...
                                                    [7: 8: 9: 1: 2: 1: 2: 3: 4: 5: 6: 3: 4: 5:  :  :  :  :  :  :  : x]   // rules:  index 4s' previous index is 11
                                                    [7: 8: 9: 1: 2:  : 2: 3: 4: 5: 6: 3: 4: 5:  :  :  :  :  :  :  : x]   // rules:  index 4s' previous index is 11, index 13s' next index is 5
                                                    [7: 8: 9: 1: 2:  :  : 3: 4: 5: 6: 3: 4: 5:  :  :  :  :  :  :  : x]   // rules:  index 4s' previous index is 11, index 13s' next index is 5
                                                    etc...
                In other words the queue always tries to fill the empty spaces to the left in the array. The datastructure keeps track of indecies by having rules for certains ones.
                The datastructure also needs to be able to remove rules when they aren't needed anymore. The rules should be kept in a hashtable for fast lookup (the hashtable should be typed)

*/

/* 3
    1 Maybe its easier to just not use realloc and always malloc and rearange all values anyway
    2 Otherwise research about data structures (maybe fibonacci heaps are relevant in this case?)
*/

typedef struct q queue;
typedef struct r rules;
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
size_t q_init(queue *q, size_t size);
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

#endif