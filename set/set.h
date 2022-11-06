#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include "../bstree/bstree.h"
#include "../datatype.h"

typedef struct _set set;

struct _set {
    bstree t;
    int (*compare)(const void*, const void*);
    void (*freeObject)(void*);
};

/**
 * @brief create new empty set
 * 
 * @return set
 */
set set_create_empty();
/**
 * @brief initialize the set
 * 
 * @param s set to initialize 
 * @param compare comparison function 
 * @param freeObject function to free an item 
 * @return true on success
 */
bool set_init(set* s, int (*compare)(const void*, const void*), void (*freeObject)(void*));
/**
 * @brief add an item to the set
 * 
 * @param s set to add to
 * @param item item to add
 * @return true if success
 * @return false if the set already contains item
 */
bool set_add(set* s, void* item);
/**
 * @brief remove an item from the set
 * 
 * @param s set to remove from
 * @param item item to remove
 * @return pointer to removed item or null if the set doesn't contain the item
 */
void* set_remove(set* s, void* item);
/**
 * @brief check if an item is in the set
 * 
 * @param s set to check
 * @param item item to search for
 * @return true if in the set
 * @return false if not in the set
 */
bool set_contains(set* s, void* item);
/**
 * @brief destroy set
 * 
 * @param s set to destroy 
 * @return true on success
 */
bool set_destroy(set* s);

#endif