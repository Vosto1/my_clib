#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../datatype.h"
#include "../utils/tscounter.h"
#include "../utils/tserror.h"
// https://stackoverflow.com/questions/3061582/include-all-files-in-a-directory

typedef enum errorc
{
    SUCCESS = 0,
    EMEM_ALLOC = 1,
    EMEM_IREALLOC = 2,
    EMEM_DREALLOC = 3,
    EINDEX_OUT_OF_BOUNDS = 4,
    EFREE_NULLPTR = 5,
    EARRDATA_DOESNT_EXIST = 6,
    EARR_EMPTY = 7,
    ENULL_ARG = 8,
    EFILE_OPENWRITE = 9,
    EFILE_OPENREAD = 10,
    EARR_INDEX_OCCUPIED = 11,
    EHNEW_KEY = 12,
    EH_DATA_DOESNT_EXIST = 13,
    EHEAP_EMPTY = 14,
    EHEAP_NULL = 15,
    EHNEWKEY_NOT_SET = 16,
    EPQ_EMPTY = 17,
    EWRITE_BINARY = 18,
    EHASHDATA_DOESNT_EXIST = 19,
    EHASH_NULL = 20,
    EHASHTABLE_OVERFLOW_MEMALLOC = 21,
    EHASHTABLE_OVERFLOW_BUFFER = 22,
    EHASH_TRIM_BUFFER = 23,
    EHASH_TRIM_MEMALLOC = 24,
    EHASHDICT = 25,
    EBTREE_NODE_MEMALLOC = 26,
    EBTREE_FREENULLPTR = 27,
    EBTREE_WRITEARR_MEMALLOC = 28,
    EBTREE_WRITEARR_EMPTY = 29,
    EBTREE_BALANCE = 30,
} errorc;

#endif