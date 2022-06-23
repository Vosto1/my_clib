#include "text_analysis.h"

#define ALPHABET 26

static int _compare(cvoidp_t e1, cvoidp_t e2)
{
    entry *en1 = (entry *)e1;
    entry *en2 = (entry *)e2;
    return (int)en1->k - (int)en2->k;
}

static dim_t _hash(cvoidp_t e, const hashtable *ht)
{
    entry *f = (entry *)e;

    dim_t index = f->k * 37;
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

hashtable letter_occurances(char *string, int strsize)
{
    hashtable ht;
    if (ht_init(&ht, ALPHABET, &_hash, &_compare) != ALPHABET)
    {
        errcset(EHASHDICT);
        return ht;
    }

    entry *e;
    entry *l;
    for (int i = 0; i < strsize; i++)
    {
        e = _create_entry(string[i], 1);
        l = ht_lookup(&ht, e);
        if (l == UNUSED)
        {
            ht_insert(&ht, e); // if the letter did not exist add to ht
        }
        else
        {
            l->v += 1; // if the letter did exist, increment value
        }
    }
    ht_trim(&ht);

    return ht;
}