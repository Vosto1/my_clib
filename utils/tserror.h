#ifndef _ERROR_H
#define _ERROR_H
// thread safe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_color.h"

#define NO_ERROR 0
#define NO_ERROR_MSG "no error"
#define ERROR_MESSAGE_MAX_LENGTH 1024

typedef unsigned int errorcode;
typedef char message;

struct err {
    errorcode errc;
    message errstr[ERROR_MESSAGE_MAX_LENGTH];
    message* (*geterror)(errorcode e);
};

typedef struct err error;

/**
 * when there has been an error, set errc variable to the error code
 *
 * @param e ErrorCode to be set
 */
void errset(error* e, errorcode errc);

/**
 * use when an error has been handled
 */
void errreset(error* e);

/**
 * Initialize error struct
 */
error errinit(message* (*geterror)(errorcode e));

#endif