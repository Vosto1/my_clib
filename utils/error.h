#ifndef _ERROR_H
#define _ERROR_H

#include "stdio.h"
#include "stdlib.h"
#include "print_color.h"

#define errc errcget()

typedef enum ErrorCodes
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
} ErrorCode;

/**
 * get the current error code
 *
 * @return ErrorCode
 */
ErrorCode errcget();

/**
 * when there has been an error, set errc variable to the error code
 *
 * @param e ErrorCode to be set
 */
void errcset(ErrorCode e);

/**
 * use when an error has been handled
 */
void errcreset();

/**
 * Initialize errorcode
 * this function is needed in the beginning of a program that uses errorcodes
 */
void errcinit();

/**
 * get error string from associated ErrorCode
 *
 * @param e ErrorCode to which the wished error string is associated
 * @return const char* error string
 */
const char *error(ErrorCode e);

/**
 * default error handler
 * this handler terminates the program on error
 */
void error_handler();

#endif