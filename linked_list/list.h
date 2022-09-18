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

/*doubly-linked list*/
struct node
{
    voidp_t data;
    struct node *next;
    struct node *previous;
};

typedef struct node *ll;

// returns an empty list
ll ll_create_empty(void);

// returns true if empty
bool ll_is_empty(const ll list);

// adds a new node first in the list
void ll_add_first(ll *list, const voidp_t data);

// adds a new node last in the list
void ll_add_last(ll *list, const voidp_t data);

// remove the node first in the list
void ll_remove_first(ll *list);

// remove the node last in the list
void ll_remove_last(ll *list);

// ta bort data ur listan (forsta forekomsten), returnera 0 om datat inte finns, annars 1
bool ll_remove_element(ll *list, const voidp_t data);

// Sok efter data i listan, returnera 1 om datat finns, annars 0.
bool ll_search(const ll list, const voidp_t data);

// returnera hur manga noder som finns i listan
int ll_node_count(const ll list);

// tom listan /ta bort allt data (alla noder) ur listan
void ll_clear(ll *list);

// returnera forsta datat i listan
voidp_t ll_get_first(const ll list);

// returnera sista datat i listan
voidp_t ll_get_last(const ll list);

#endif
