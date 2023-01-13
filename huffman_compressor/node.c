#include "node.h"

int node_compare(const void * e1, const void * e2)
{
    node n1 = (node)e1;
    node n2 = (node)e2;
    return node_entry_compare(n1->value, n2->value);
}

int node_entry_compare(const void * e1, const void * e2)
{
    entry *en1 = (entry *)e1;
    entry *en2 = (entry *)e2;
    return (int)en1->k - (int)en2->k;
}

uint node_hash(const void * e, const hashtable *ht)
{
    entry *f = (entry *)e;

    int index = f->k * 37;
    index %= ht_size(ht);
    return index;
}

node node_create(entry* e)
{
    return bt_new_node(e);
}

entry *node_create_entry(key k, value v)
{
    entry *e = (entry *)malloc(sizeof(entry));
    e->k = k;
    e->v = v;
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