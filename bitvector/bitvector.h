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
#include "../utils/error.h"
#include "../s_dynamic_array/sdarray.h"

typedef unsigned char byte;
typedef unsigned long long dim_t;
typedef sdarray bitvector;

typedef struct
{
    unsigned int residualBits;
    unsigned int amountOfBytes;
    byte *bytes;
} binary;

dim_t bit_count(bitvector *bv);
bitvector bv_create_empty();
dim_t bv_init(bitvector *bv);
dim_t bv_add(bitvector *bv, bool value);
dim_t bv_merge(bitvector *bv, bitvector *unionWith);
bool bv_remove_last(bitvector *bv);
bool *bv_at(bitvector *bv, int index);
dim_t bv_clear(bitvector *bv);
void bv_delete(bitvector *bv);
unsigned int bools2bits(bitvector *bv, binary *out);
bool bits2bools(binary *b, bitvector *out);
dim_t write_binary_to_file(binary *b, char *file);
dim_t read_binary_from_file(char *file, binary *b);

void printbitvector(bitvector *bv);
void printbinary(binary *bin);

#endif
