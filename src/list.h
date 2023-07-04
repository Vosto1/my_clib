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
#include "datatype.h"

/*doubly-linked list*/
typedef struct node
{
    const void* data;
    struct node *next;
    struct node *previous;
}node;

struct head {
    node* first;
    node* last;
};
typedef struct head linkedlist;

// returns an empty list
linkedlist ll_create_empty(void);

// returns true if empty
bool ll_is_empty(linkedlist* head);

// adds a new node first in the list
void ll_add_first(linkedlist* head, const void* data);

// adds a new node last in the list
void ll_add_last(linkedlist* head, const void* data);

// remove the node first in the list
const void* ll_remove_first(linkedlist* head);

// remove the node last in the list
const void* ll_remove_last(linkedlist* head);

// ta bort data ur listan (forsta forekomsten), returnera 0 om datat inte finns, annars 1
const void* ll_remove_element(linkedlist* head, const void* data, int (*compare)(const void*, const void*));

// Sok efter data i listan, returnera 1 om datat finns, annars 0.
const void* ll_search(linkedlist* head, const void* data, int (*compare)(const void*, const void*));

// returnera hur manga noder som finns i listan
uint ll_node_count(linkedlist* head);

// tom listan /ta bort allt data (alla noder) ur listan
void ll_clear(linkedlist* head, void (*freeObject)(void*));

// returnera forsta datat i listan
const void* ll_get_first(linkedlist* head);

// returnera sista datat i listan
const void* ll_get_last(linkedlist* head);

#endif
