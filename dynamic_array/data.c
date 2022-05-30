#include "data.h"

resultType compareData(Data x, Data y) {
    return x.value - y.value;
}

void printData(size_t used, Data a[]) {
    for (int i = 0; i < used; i++) {
        printf("value at %d: %d\n", i, a[i].value);
    } printf("\n");
}