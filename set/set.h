#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include "../bstree/bstree.h"
#include "../datatype.h"

typedef bstree set;

/**
 * @brief add an item to the set
 * 
 * @param s set to add to
 * @param item item to add
 * @return true if success
 * @return false if failure
 */
bool set_add(set* s, cvoidp_t item);
/**
 * @brief remove an item from the set
 * 
 * @param s set to remove from
 * @param item item to remove
 * @return pointer to removed item or null if error
 */
cvoidp_t set_remove(set* s, cvoidp_t item);
/**
 * @brief check if an item is in the set
 * 
 * @param s set to check
 * @param item item to search for
 * @return true if in the set
 * @return false if not in the set
 */
bool set_contains(set* s, cvoidp_t item);

#endif