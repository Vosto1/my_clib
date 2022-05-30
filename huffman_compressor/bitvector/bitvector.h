/*
en lista av tex. bools som representerar bitarna som ska skrivas till fil (kan använda dynamiska array eller linked list för listan)

add // lägg till ny bool(== representation för en bit)
union // lägg ihop två bit vector (appenda den ena i slutet av den andra)
*/
#ifndef BITVECTOR_H
#define BITVECTOR_H

#include "../utils/file.h"
#include "../utils/error.h"
#include "dynamic_array.h"
#include <stdbool.h>

typedef char byte;


typedef dynamicArray bitVector;

typedef struct {
    unsigned int residualBits;
    unsigned int amountOfBytes;
    byte* bytes;
}binary;


size_t bitCount(bitVector* bv);
bitVector createEmptyBitVector();
ErrorCode initBitVector(bitVector* bv);
ErrorCode addbv(bitVector* bv, bool value);
ErrorCode unionbv(bitVector* bv, bitVector* unionWith);
ErrorCode removeLastbv(bitVector* bv);
ErrorCode clearbv(bitVector* bv);
unsigned int sizeBits(unsigned int size);
ErrorCode bools2bits(bitVector* bv, binary* out);
ErrorCode bits2bools(binary* b, bitVector* out);
void dbug(byte* binary, size_t amountOfBytes);
ErrorCode writeBinaryFromFile(binary* b, char* file);
ErrorCode readBinaryFromFile(char* filename, binary* b);


#endif