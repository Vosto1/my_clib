#ifndef HASH_H
#define HASH_H

#include "../datatype.h"

#define HASH size_t
#define UCHAR unsigned char

HASH hash(void * data, int byteSize);

#endif