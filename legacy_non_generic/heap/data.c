#include "data.h"


// dynamic array
resultType compareData(Data x, Data y) {
    return x.key - y.key;
}

void printData(size_t used, Data a[]) {
    for (int i = 0; i < used; i++) {
        printf("value at %d: [%f;%c]\n", i, a[i].key, a[i].element);
    } printf("\n");
}

// heap
Key decrementKey(Key key) {
    return key - 1.0;
}