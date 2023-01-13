#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "../dynamic_array/darray.h"
#include "../datatype.h"

typedef darray stack;
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
 * @return int 
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

#endif