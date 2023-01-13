#include "node.h"

//used in the pq occurances is the comparison value
int node_compare(const void * o1, const void * o2)
{
    node n1 = (node)o1;
    node n2 = (node)o2;
    entry* e1 = (entry*)n1->value;
    entry* e2 = (entry*)n2->value;
    return e1->value - e2->value;
}

//used in the hashtable, unique characters are needed hence the char/byte is the comparison value
int node_entry_compare(const void * e1, const void * e2)
{
    entry *en1 = (entry *)e1;
    entry *en2 = (entry *)e2;
    return (int)en1->key - (int)en2->key;
}

uint node_hash(const void * e, const hashtable *ht)
{
    entry *f = (entry *)e;

    int index = f->key * 37;
    index %= ht_size(ht);
    return index;
}

node node_create(entry* e)
{
    return bt_new_node(e);
}

entry *node_create_entry(byte key, int value, bool branch)
{
    entry *e = (entry *)malloc(sizeof(entry));
    e->key = key;
    e->value = value;
    e->branch = branch;
    return e;
}

void node_free_object(void * o)
{
    node n = (node)o;
    node_free_entry_object(n->value);
    free((void*)n);
}

void node_free_entry_object(void * o)
{
    free(o);
}