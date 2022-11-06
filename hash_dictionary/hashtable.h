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
//#include "../utils/error.h"
#include "../s_dynamic_array/sdarray.h"
#include "../datatype.h"

#define UNUSED NULL

typedef struct htabl hashtable;
struct htabl
{
    size_t (*hash)(const void*, const hashtable *);
    int (*compare)(const void*, const void*);
    void (*freeObject)(void*);
    void* *entries;
    size_t size;
};

hashtable ht_create_empty();
size_t ht_init(hashtable *ht, size_t size, size_t (*hash)(const void*, const hashtable *), int (*compare)(const void*, const void*), void (*freeObject)(void*));
bool ht_destroy(hashtable *ht);
bool ht_free(hashtable *ht);
size_t ht_trim(hashtable *ht);

int ht_insert(hashtable *ht, void* element);
void* ht_delete(hashtable *ht, const void* element);
void* ht_lookup(const hashtable *ht, const void* element);

size_t ht_size(const hashtable *ht);
size_t ht_count(const hashtable *ht);

#endif