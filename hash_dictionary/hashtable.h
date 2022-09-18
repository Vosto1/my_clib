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
#include "../utils/error.h"
#include "../s_dynamic_array/sdarray.h"
#include "../datatype.h"

#define UNUSED NULL

typedef struct htabl hashtable;
struct htabl
{
    dim_t (*hash)(cvoidp_t, const hashtable *);
    int (*compare)(cvoidp_t, cvoidp_t);
    voidp_t *entries;
    dim_t size;
};

hashtable ht_create_empty();
dim_t ht_init(hashtable *ht, dim_t size, dim_t (*hash)(cvoidp_t, const hashtable *), int (*compare)(cvoidp_t, cvoidp_t));
void ht_destroy(hashtable *ht);
void ht_free(hashtable *ht);
dim_t ht_trim(hashtable *ht);

int ht_insert(hashtable *ht, voidp_t element);
voidp_t ht_delete(hashtable *ht, cvoidp_t element);
voidp_t ht_lookup(const hashtable *ht, cvoidp_t element);

dim_t ht_size(const hashtable *ht);
dim_t ht_count(const hashtable *ht);

#endif