#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "../utils/error.h"
#include "../s_dynamic_array/s_dynamic_array.h"

#define UNUSED NULL

typedef void* voidp;
typedef const void* cvoidp;
typedef unsigned long long size_t;

typedef struct htabl hashtable;
struct htabl {
    size_t (*hash)(cvoidp,const hashtable*);
    int (*compare)(cvoidp,cvoidp);
    voidp* entries;
    size_t size;
};

hashtable ht_createEmpty();
size_t ht_init(hashtable* ht, size_t size, size_t (*hash)(cvoidp,const hashtable*), int (*compare)(cvoidp,cvoidp));
void ht_free(hashtable* ht);

size_t ht_insert(hashtable* ht, voidp element);
voidp ht_delete(hashtable* ht, cvoidp element);
voidp ht_lookup(const hashtable* ht, cvoidp element);

size_t ht_size(const hashtable* ht);
size_t ht_count(const hashtable* ht);

static size_t indexof(hashtable* ht, cvoidp valueToSearchFor);
static size_t linear_probe(hashtable* ht, voidp element);

#endif