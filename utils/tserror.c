#include "tserror.h"

void errset(error* e, errorcode errc)
{
    e->errc = errc;
    e->errstr = (*e->geterror)(errc);
}

void errreset(error* e)
{
    e->errc = NO_ERROR;
    e->errstr = NO_ERROR_MSG;
}

void errinit(errorstring (*geterror)(errorcode e))
{
    error e;
    e.errc = NO_ERROR;
    e.errstr = NO_ERROR_MSG;
    e.geterror = geterror;
    return e;
}