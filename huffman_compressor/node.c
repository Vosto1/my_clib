#include "node.h"

uint encode_rule_hash(const void * o, const hashtable *ht)
{
    encodeRule *er = (encodeRule*)o;

    int index = er->data * 37;
    index %= ht_size(ht);
    return index;
}

int encode_rule_compare(const void* o1, const void* o2)
{
    encodeRule* er1 = (encodeRule*)o1;
    encodeRule* er2 = (encodeRule*)o2;
    return (int)er1->data - (int)er2->data;
}

encodeRule* encode_rule_create(byte key, bitvector code)
{
    encodeRule* er = (encodeRule*)malloc(sizeof(encodeRule));
    er->code = code;
    er->data = key;
    return er;
}

void encode_rule_free(void* o)
{
    encodeRule* er = (encodeRule*)o;
    assert(bv_delete(&er->code));
    free(er);
}

void encode_rule_print(void* o)
{
    encodeRule* e = (encodeRule*)o;
    printf("[");
    printf("%c", e->data);
    printf(",");
    print_bitvector_concise(&e->code);
    printf("]");
}

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
    if (e == NULL)
    {
        printf("node create failed\n");
        return NULL;
    }
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

void node_print_entry(void* o)
{
    entry *e = (entry *)o;
    printf("[%c,%d]", e->key, e->value);
}