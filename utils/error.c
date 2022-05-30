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
        case 0:
            return "no error";
            break;
        case 1:
            return "mem alloc";
            break;
        case 2:
            return "mem increase realloc";
            break;
        case 3:
            return "mem decrease realloc";
            break;
        case 4:
            return "index out of bounds";
            break;
        case 5:
            return "free memory was null pointer";
            break;
        case 6:
            return "data does not exist";
            break;
        case 7:
            return "array empty";
            break;
        case 8:
            return "null argument";
            break;
        case 9:
            return "new key was larger or equal to the old key";
            break;
        case 10:
            return "data did not exist";
            break;
        case 11:
            return "priority queue is empty";
            break;
        case 12:
            return "couldn't write bitvector to file";
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