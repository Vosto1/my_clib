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
#include "../utils/file.h"
//#include "../utils/error.h"
#include "../s_dynamic_array/sdarray.h"
#include "../datatype.h"


typedef sdarray bitvector;
typedef struct b
{
    unsigned int residualBits;
    unsigned int amountOfBytes;
    byte *bytes;
}binary;

uint bit_count(bitvector *bv);
bitvector bv_create_empty();
int bv_init(bitvector *bv);
uint bv_add(bitvector *bv, bool value);
uint bv_merge(bitvector *bv, bitvector *unionWith);
bool bv_remove_last(bitvector *bv);
bool *bv_at(bitvector *bv, int index);
uint bv_clear(bitvector *bv);
bool bv_delete(bitvector *bv);
unsigned int bools2bits(bitvector *bv, binary *out);
bool bits2bools(binary *b, bitvector *out);
uint write_binary_to_file(binary *b, char *file);
uint read_binary_from_file(char *file, binary *b);

void printbitvector(bitvector *bv);
void printbinary(binary *bin);

binary bin_new();
bool bin_equal(binary x, binary y);
unsigned int bin_amount_bytes(binary x);
bool bin_free(binary x);

#endif
