#include "error.h"

ErrorCode errcode;

ErrorCode errcget() {
    return errcode;
}
void errcset(ErrorCode e) {
     errcode = e;
}
void errcreset() {
    errcode = SUCCESS;
}
void errcinit() {
    errcode = SUCCESS;
}

const char* error(ErrorCode e) {
    switch (e) {
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
        default:
            printf("Error: %d", e);
            return " unknown error";
    }
}


void errorHandler() {
    if (errc != SUCCESS) {
        fprintf(stderr, error(errc));
        exit(errc);
    } else return;
}