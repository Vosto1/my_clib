#include "tserror.h"

void errset(error* e, errorcode errc)
{
    e->errc = errc;
    e->errstr[0] = '\0';
    strncat(e->errstr, (*e->geterror)(errc), ERROR_MESSAGE_MAX_LENGTH - 1);
}

void errreset(error* e)
{
    e->errc = NO_ERROR;
    strcpy(e->errstr, NO_ERROR_MSG);
}

error errinit(message* (*geterror)(errorcode e))
{
    error e;
    e.errc = NO_ERROR;
    strcpy(e.errstr, NO_ERROR_MSG);
    e.geterror = geterror;
    return e;
}