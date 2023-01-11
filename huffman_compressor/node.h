#ifndef NODE_H
#define NODE_H

#include "../hash_dictionary/hashtable.h"

typedef int value;
typedef unsigned char key;
typedef struct
{
    value v;
    key k;
} entry;

#endif