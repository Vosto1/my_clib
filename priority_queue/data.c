#include "data.h"


// dynamic array
resultType compareData(Data x, Data y) {
    return x.p - y.p;
}

void printData(size_t used, Data a[]) {
     for (int i = 0; i < used; i++) {
        printf("value at %d: [%d;%c]\n", i, a[i].p, a[i].e);
    } printf("\n");
}

// heap
resultType decrementKey(Data data) {
    return data.p - 1.0;
}