#include "text_analysis.h"

#define ALPHABET 26

static int compare(cvoidp e1, cvoidp e2) {
    entry* en1 = (entry*)e1;
    entry* en2 = (entry*)e2;
    return (int)en1->k - (int)en2->k;
}

static size_t hash(cvoidp e, const hashtable* ht) {
    entry* f = (entry*)e;
    
    size_t index = f->k * 37;
    index %= ht_size(ht);
    return index;
}

static entry* createEntry(key k, value v) {
    entry* e = (entry*)malloc(sizeof(entry));
    e->k = k;
    e->v = v;
    return e;
}

hashtable letterOccurances(char* string, int strsize) {
    hashtable ht;
    if (ht_init(&ht, ALPHABET, &hash, &compare) != ALPHABET) {
        errcset(EHASHDICT);
        return ht;
    }

    entry* e;
    entry* l;
    for (int i = 0; i < strsize; i++) {
        e = createEntry(string[i], 1);
        l = ht_lookup(&ht, e);
        if (l == UNUSED) {
            ht_insert(&ht, e); // if the letter did not exist add to ht
        } else {
            l->v += 1; // if the letter did exist, increment value
        }
    }
    ht_trim(&ht);
    
    return ht;
}