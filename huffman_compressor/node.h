#ifndef NODE_H
#define NODE_H

#include "../hash_dictionary/hashtable.h"
#include "../btree/btree.h"
#include "../bitvector/bitvector.h"

typedef struct
{
    int value;
    byte key;
    bool branch;
} entry;

typedef struct 
{
    byte data;
    bitvector code;
}encodeRule;

typedef btree node;

uint encode_rule_hash(const void * o, const hashtable *ht);
int encode_rule_compare(const void* o1, const void* o2);
encodeRule* encode_rule_create(byte key, bitvector code);
void encode_rule_free(void* o);
int node_entry_compare(const void * e1, const void * e2);
int node_compare(const void * e1, const void * e2);
uint node_hash(const void * e, const hashtable *ht);
node node_create(entry* e);
entry *node_create_entry(byte key, int value, bool branch);
void node_free_object(void * e);
void node_free_entry_object(void * e);
#endif