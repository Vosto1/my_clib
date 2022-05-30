#ifndef _ERROR_H
#define _ERROR_H 

#include "stdio.h"
#include "stdlib.h"

#define errc errcget()

/* if (errcget() == SUCCESS) {
        return initSize;
    } else {
        errcset("error code here");
    } */

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
    ENEW_KEY = 9,
    EH_DATA_DOESNT_EXIST = 10,
    EPQ_EMPTY = 11,
    EWRITE_BINARY = 12,
    EFILE_OPENWRITE = 13,
    EFILE_OPENREAD = 14,
    EARR_INDEX_OCCUPIED = 15,
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