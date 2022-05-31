#ifndef _ERROR_H
#define _ERROR_H 

#include "stdio.h"
#include "stdlib.h"

#define errc errcget()

typedef enum ErrorCodes {
    SUCCESS = 0,
    EMEM_ALLOC = 1,
    EMEM_IREALLOC = 2,
    EMEM_DREALLOC = 3,
    EINDEX_OUT_OF_BOUNDS = 4,
    EFREE_NULLPTR = 5,
    EDATA_NEXIST = 6,
    EARR_EMPTY = 7,
    ENULL_ARG = 8,
    EWRITE_BINARY = 9,
    EFILE_OPENWRITE = 10,
    EFILE_OPENREAD = 11,
    EARR_INDEX_OCCUPIED = 12,
    EHNEW_KEY = 13,
    EH_DATA_DOESNT_EXIST = 14,
    EHEAP_EMPTY = 15,
    EPQ_EMPTY = -1,
}ErrorCode;

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
const char* error(ErrorCode e);

/**
 * default error handler
 * this handler terminates the program on error
 */
void errorHandler();

#endif