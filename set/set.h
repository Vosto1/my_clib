#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include "../bstree/bstree.h"
#include "../datatype.h"

typedef struct _set set;

struct _set {
    bstree t;
    int (*compare)(cvoidp_t, cvoidp_t);
};

/**
 * @brief create new empty set
 * 
 * @return set
 */
set st_create_empty();
/**
 * @brief add an item to the set
 * 
 * @param s set to add to
 * @param item item to add
 * @return true if success
 * @return false if the set already contains item
 */
bool set_add(set* s, voidp_t item);
/**
 * @brief remove an item from the set
 * 
 * @param s set to remove from
 * @param item item to remove
 * @return pointer to removed item or null if the set doesn't contain the item
 */
voidp_t set_remove(set* s, voidp_t item);
/**
 * @brief check if an item is in the set
 * 
 * @param s set to check
 * @param item item to search for
 * @return true if in the set
 * @return false if not in the set
 */
bool set_contains(set* s, voidp_t item);

#endif