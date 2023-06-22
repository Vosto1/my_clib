#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "mdarray.h"
#include "datatype.h"

typedef mdarray stack;
/**
 * @brief create new empty stack
 * 
 */
stack st_create_empty();
/**
 * @brief 
 * 
 * @param s stack to initialize
 * @param init_size initial size of the stack
 * @param freeObject a function that can free an item that will be in the stack
 * @return initialized size 
 */
int st_init(stack* s, uint init_size, void (*freeObject)(void*));
/**
 * @brief get item count in stack
 * 
 */
uint st_count(stack* s);
/**
 * @brief remove all items in stack
 * 
 */
int st_clear(stack* s);
/**
 * @brief peek at item on the top of the stack
 * 
 */
const void* st_peek(stack* s);
/**
 * @brief put a new item on the top of the stack
 * 
 */
bool st_push(stack* s, const void* item);
/**
 * @brief remove the item at the top of the stack
 * 
 */
void* st_pop(stack* s);
/**
 * @brief check if a stack is empty
 * 
 * @param s stack to check
 * @return true if empty
 * @return false
 */
bool st_is_empty(stack* s);
/**
 * @brief free a stack and it's contents
 * 
 * @param s stack to free
 * @return true on success
 * @return false 
 */
bool st_free(stack* s);
/**
 * @brief free a stack without freeing the items within the stack
 * 
 * @param s stack to free
 * @return true on success
 * @return false 
 */
bool st_destroy(stack* s);

#endif