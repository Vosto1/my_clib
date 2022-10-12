#include "test.h"

void i(counter *c)
{
    (*c->INC)(c);
}

void r(counter *c)
{
    (*c->RST)(c);
}

uint c(counter c)
{
    return (*c.CNT)(c);
}

void tscounter_test()
{
    counter n = new_counter();
    printf("%d", c(n));
    for (int j = 0; j < 5; j++) i(&n);
    printf("%d", c(n));
    r(&n);
    printf("%d", c(n));
}

message* _error(errorcode e)
{
    switch (e)
    {
    case SUCCESS:
        return "no error";
    case EMEM_ALLOC:
        return "mem alloc";
    case EMEM_IREALLOC:
        return "mem increase realloc";
    case EMEM_DREALLOC:
        return "mem decrease realloc";
    case EINDEX_OUT_OF_BOUNDS:
        return "index out of bounds";
    case EFREE_NULLPTR:
        return "free memory was null pointer";
    case EARRDATA_DOESNT_EXIST:
        return "data does not exist";
    case EARR_EMPTY:
        return "array empty";
    case ENULL_ARG:
        return "null argument";
    case EFILE_OPENWRITE:
        return "couln't open file to write";
    case EFILE_OPENREAD:
        return "couln't open file to read";
    case EARR_INDEX_OCCUPIED:
        return "index was occupied";
    case EHNEW_KEY:
        return "new key was larger or equal to the old key";
    case EH_DATA_DOESNT_EXIST:
        return "heap data did not exist";
    case EHEAP_EMPTY:
        return "heap was empty";
    case EHEAP_NULL:
        return "heap was null";
    case EHNEWKEY_NOT_SET:
        return "couldn't set the new key on the item";
    case EPQ_EMPTY:
        return "priority queue is empty";
    case EWRITE_BINARY:
        return "couldn't write bitvector to file";
    case EHASHDATA_DOESNT_EXIST:
        return "data wasn't present in the hashtable";
    case EHASH_NULL:
        return "hashtable was null";
    case EHASHTABLE_OVERFLOW_MEMALLOC:
        return "fatal error, realloc failed during hashtable overflow";
    case EHASHTABLE_OVERFLOW_BUFFER:
        return "couldn't create buffer for elements to reallocate hashtable memory";
    case EHASH_TRIM_BUFFER:
        return "couldn't create buffer for elements to trim hashtable memory";
    case EHASH_TRIM_MEMALLOC:
        return "fatal error, realloc failed during hashtable memory trim";
    case EHASHDICT:
        return "hashdictionary couldn't be initialized for letter occurances";
    case EBTREE_NODE_MEMALLOC:
        return "couldn't create node";
    case EBTREE_FREENULLPTR:
        return "free memory was null pointer";
    case EBTREE_WRITEARR_MEMALLOC:
        return "write tree to array, memory allocation for the array failed";
    case EBTREE_WRITEARR_EMPTY:
        return "write tree to array, tree was empty";
    case EBTREE_BALANCE:
        return "balance tree failed";
    default:
        printf("Error: %d", e);
        return " unknown error";
    }
}

void error_handler(error e)
{
    fprintf(stderr, "%s. E%d\n", e.errstr, e.errc);
}

errorcode random_error()
{
    return rand() % 31; // 30 == amount of errors defined with codes: 0 -> 30
}

void separator()
{
    printf("\n------------------------------------------------------------------\n");
}

void tserror_test()
{
    srand(time(NULL));
    error e = errinit(_error);
    error_handler(e);
    separator();
    
    for (int i = 0; i < 10; i++)
    {
        errset(&e, random_error());
        error_handler(e);
        separator();
    }
}



int main(void)
{
    //tscounter_test();
    tserror_test();


    return 0;
}