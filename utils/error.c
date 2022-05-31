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
            break;
        case EMEM_ALLOC:
            return "mem alloc";
            break;
        case EMEM_IREALLOC:
            return "mem increase realloc";
            break;
        case EMEM_DREALLOC:
            return "mem decrease realloc";
            break;
        case EINDEX_OUT_OF_BOUNDS:
            return "index out of bounds";
            break;
        case EFREE_NULLPTR:
            return "free memory was null pointer";
            break;
        case EDATA_NEXIST:
            return "data does not exist";
            break;
        case EARR_EMPTY:
            return "array empty";
            break;
        case ENULL_ARG:
            return "null argument";
            break;
        case EWRITE_BINARY:
            return "couldn't write bitvector to file";
            break;
        case EFILE_OPENWRITE:
            return "couln't open file to write";
            break;
        case EFILE_OPENREAD:
            return "couln't open file to read";
            break;
        case EARR_INDEX_OCCUPIED:
            return "index was occupied";
            break;
        case EHNEW_KEY:
            return "new key was larger or equal to the old key";
            break;
        case EH_DATA_DOESNT_EXIST:
            return "heap data did not exist";
            break;
        case EHEAP_EMPTY:
            return "heap was empty";
            break;
        case EPQ_EMPTY:
            return "priority queue is empty";
            break;
        default:
            printf("Error: %d", e);
            return " unknown error";
            break;
    }
}


void errorHandler() {
    if (errc != SUCCESS) {
        fprintf(stderr, error(errc));
        exit(errc);
    } else return;
}