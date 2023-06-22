/**
 * @file bitvector.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <stdbool.h>
#include <stdio.h>
#include "file.h"
#include "mdarray.h"
#include "datatype.h"

#define ERRWRITE -1

typedef mdarray bitvector;
typedef struct b
{
    uint residualBits;
    uint amountOfBytes;
    byte *bytes;
}binary;

uint bit_count(bitvector *bv);
bitvector bv_create_empty();
bool bv_init(bitvector *bv);
int bv_add(bitvector *bv, bool value);
int bv_merge(bitvector *bv, bitvector *unionWith);
bitvector bv_duplicate(bitvector* bv); // duplicate the bitvector sent to the function, copy is returned.
bool bv_remove_last(bitvector *bv);
bool *bv_at(bitvector *bv, uint index);
uint bv_clear(bitvector *bv);
bool bv_delete(bitvector *bv);
int bv_bools2bits(bitvector *bv, binary *out);
bool bv_bits2bools(binary *b, bitvector *out);
int bv_write_binary_to_file(binary *b, char *file);
int bv_read_binary_from_file(char *file, binary *b);

void bv_print_bitvector(bitvector *bv);
void bv_print_bitvector_concise(bitvector* bv);
void bin_print(binary *bin);

binary bin_new();
bool bin_equal(binary x, binary y);
uint bin_amount_bytes(binary x);
bool bin_free(binary x);

#endif
