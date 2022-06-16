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
#include "../s_dynamic_array/s_dynamic_array.h"

typedef unsigned char byte;
typedef unsigned long long size_t;
typedef s_dynamicArray bitvector;

typedef struct
{
    unsigned int residualBits;
    unsigned int amountOfBytes;
    byte *bytes;
} binary;

size_t bitCount(bitvector *bv);
bitvector bvcreateEmpty();
size_t bvinit(bitvector *bv);
size_t bvadd(bitvector *bv, bool value);
size_t bvmerge(bitvector *bv, bitvector *unionWith);
bool bvremoveLast(bitvector *bv);
size_t bvclear(bitvector *bv);
bool *bvat(bitvector *bv, int index);
unsigned int bools2bits(bitvector *bv, binary *out);
size_t bits2bools(binary *b, bitvector *out);
size_t writeBinaryToFile(binary *b, char *file);
size_t readBinaryFromFile(char *file, binary *b);

static unsigned int sizeBits(unsigned int bytes);
void printbitvector(bitvector *bv);
void printbinary(binary *bin);

#endif