#include "data.h"

comparisonValue compareData(Data x, Data y) {
    return (int)x - (int)y;
}

void printData(size_t used, Data a[]) {
    for (int i = 0; i < used; i++) {
        if (a[i])
            printf("value at %d: [true]\n", i);
        else
            printf("value at %d: [false]\n", i);
    } printf("\n");
}