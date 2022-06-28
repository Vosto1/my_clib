#ifndef HASH_H
#define HASH_H

typedef unsigned long long dim_t;
#define HASH dim_t
#define UCHAR unsigned char

HASH hash(void * data, int byteSize);

#endif