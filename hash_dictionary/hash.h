#ifndef HASH_H
#define HASH_H

typedef unsigned long long size_t;
#define HASH size_t
#define UCHAR unsigned char

HASH hash(void * data, int byteSize);

#endif