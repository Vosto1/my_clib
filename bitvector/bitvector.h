/*
en lista av tex. bools som representerar bitarna som ska skrivas till fil (kan använda dynamiska array eller linked list för listan)

add // lägg till ny bool(== representation för en bit)
union // lägg ihop två bit vector (appenda den ena i slutet av den andra)
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
typedef dynamicArray bitvector;

typedef struct {
    unsigned int residualBits;
    unsigned int amountOfBytes;
    byte* bytes;
}binary;

size_t bitCount(bitvector* bv);
bitvector bvcreateEmpty();
size_t bvinit(bitvector* bv);
size_t bvadd(bitvector* bv, bool value);
size_t bvunion(bitvector* bv, bitvector* unionWith);
bool bvremoveLast(bitvector* bv);
size_t bvclear(bitvector* bv);
bool* bvat(bitvector *bv, int index);
unsigned int bools2bits(bitvector* bv, binary* out);
size_t bits2bools(binary* b, bitvector* out);
size_t writeBinaryToFile(binary* b, char* file);
size_t readBinaryFromFile(char* file, binary* b);

static unsigned int sizeBits(unsigned int bytes);
void printbitvector(bitvector* bv);
void printbinary(binary* bin);

#endif