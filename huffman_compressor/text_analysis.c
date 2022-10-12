#include "text_analysis.h"

#define ALPHABET 26

static int _compare(cvoidp_t e1, cvoidp_t e2)
{
    entry *en1 = (entry *)e1;
    entry *en2 = (entry *)e2;
    return (int)en1->k - (int)en2->k;
}

static size_t _hash(cvoidp_t e, const hashtable *ht)
{
    entry *f = (entry *)e;

    size_t index = f->k * 37;
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

static void freeObject(voidp_t e)
{
    free(e);
}

hashtable letter_occurances(char *string, int strsize)
{
    hashtable ht;
    if (ht_init(&ht, ALPHABET, &_hash, &_compare, &freeObject) != ALPHABET)
    {
        errcset(EHASHDICT);
        return ht;
    }

    entry *e;
    entry *l;
    for (int i = 0; i < strsize; i++)
    {
        e = _create_entry(string[i], 1);
        l = ht_lookup(&ht, e); // the hash function is run only on the character --> i.e. the look up function checks if the character is already in the ht
        if (l == UNUSED)
        {
            ht_insert(&ht, e); // if the letter did not exist add to ht
        }
        else
        {
            l->v += 1; // if the letter existed, increment value
        }
    }
    ht_trim(&ht);

    return ht;
}