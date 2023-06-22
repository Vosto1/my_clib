/**
 * @file hashtable.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-06-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mdarray.h"
#include "datatype.h"

#define UNUSED NULL

typedef struct htabl hashtable;
struct htabl
{
    uint (*hash)(const void*, const hashtable *);
    int (*compare)(const void*, const void*);
    void (*freeObject)(void*);
    void* *entries;
    int size;
};

hashtable ht_create_empty();
int ht_init(hashtable *ht, int size, uint (*hash)(const void*, const hashtable *), int (*compare)(const void*, const void*), void (*freeObject)(void*));
bool ht_destroy(hashtable *ht);
bool ht_free(hashtable *ht);
int ht_trim(hashtable *ht);
mdarray ht_to_array(hashtable *ht);
hashtable ht_from_array(mdarray* a, uint (*hash)(const void*, const hashtable *), int (*compare)(const void*, const void*));

int ht_insert(hashtable *ht, void* element);
void* ht_delete(hashtable *ht, const void* element);
void* ht_lookup(const hashtable *ht, const void* element);

int ht_size(const hashtable *ht);
int ht_count(const hashtable *ht);

#endif