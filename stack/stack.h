#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "../s_dynamic_array/sdarray.h"
#include "../datatype.h"

typedef sdarray stack;
/**
 * @brief get item count in stack
 * 
 */
dim_t st_count(stack* s);
/**
 * @brief create new empty stack
 * 
 */
stack st_create_empty();
/**
 * @brief remove all items in stack
 * 
 */
dim_t st_clear(stack* s);
/**
 * @brief peek at item on the top of the stack
 * 
 */
cvoidp_t st_peek(stack* s);
/**
 * @brief put a new item on the top of the stack
 * 
 */
bool st_push(stack* s, cvoidp_t item);
/**
 * @brief remove the item at the top of the stack
 * 
 */
voidp_t st_pop(stack* s);

#endif