#ifndef NODE_H
#define NODE_H

#include "../hash_dictionary/hashtable.h"
#include "../btree/btree.h"

typedef int value;
typedef unsigned char key;
typedef struct
{
    value v;
    key k;
} entry;

typedef btree node;

int node_entry_compare(const void * e1, const void * e2);
int node_compare(const void * e1, const void * e2);
uint node_hash(const void * e, const hashtable *ht);
node node_create(entry* e);
entry *node_create_entry(key k, value v);
void node_free_object(void * e);
void node_free_entry_object(void * e);
#endif