/**
 * @file list.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LIST_H
#define LIST_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../datatype.h"

typedef struct head *ll;

// returns an empty list
ll ll_create_empty(void);

// returns true if empty
bool ll_is_empty(const ll* head);

// adds a new node first in the list
void ll_add_first(ll *head, cvoidp_t data);

// adds a new node last in the list
void ll_add_last(ll *head, cvoidp_t data);

// remove the node first in the list
cvoidp_t ll_remove_first(ll *head);

// remove the node last in the list
cvoidp_t ll_remove_last(ll *head);

// ta bort data ur listan (forsta forekomsten), returnera 0 om datat inte finns, annars 1
cvoidp_t ll_remove_element(ll *head, cvoidp_t data, int (*compare)(cvoidp_t, cvoidp_t));

// Sok efter data i listan, returnera 1 om datat finns, annars 0.
cvoidp_t ll_search(const ll* head, cvoidp_t data, int (*compare)(cvoidp_t, cvoidp_t));

// returnera hur manga noder som finns i listan
int ll_node_count(const ll* head);

// tom listan /ta bort allt data (alla noder) ur listan
void ll_clear(ll *head, void (*freeObject)(voidp_t));

// returnera forsta datat i listan
cvoidp_t ll_get_first(const ll* head);

// returnera sista datat i listan
cvoidp_t ll_get_last(const ll* head);

#endif
