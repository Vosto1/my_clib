#include "node.h"

static int _compare(void * e1, void * e2)
{
    entry *en1 = (entry *)e1;
    entry *en2 = (entry *)e2;
    return (int)en1->k - (int)en2->k;
}

static unsigned int _hash(void * e, const hashtable *ht)
{
    entry *f = (entry *)e;

    int index = f->k * 37;
    index %= ht_size(ht);
    return index;
}

static entry *_create_entry(key k, value v)
{
    entry *e = (entry *)malloc(sizeof(entry));
    e->k = k;
    e->v = v;
    return e;
}

static void freeObject(void * e)
{
    free(e);
}