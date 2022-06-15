#include "hash.h"
// generic hash function
// https://www.codeproject.com/Questions/5315752/How-do-I-write-a-hash-function-in-Cplusplus-that-a
HASH hash( void * data, int byteSize )
{
    HASH hv = 0x0030507; // an initial value, could be anything
    UCHAR * pd = (UCHAR *) data;
    for( int n = 0; n < byteSize; ++n )
    {
        hv << 4;
        hv += pd[ n ];
    }
    return hv;
}