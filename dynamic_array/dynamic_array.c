#include "dynamic_array.h"

dynamicArray createEmptyDynamicArray() {
    dynamicArray a;
    a.array = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

size_t initDynamicArray(dynamicArray* a, size_t initSize) {
    a->size = 0;
    a->used = 0;
    Data* temp;
    temp = (Data*)malloc(sizeof(Data)*initSize);
    if (temp != NULL) {
        a->array = temp;
        a->size = initSize;
        return initSize;
    } else {
        a->array = NULL;
        errcset(EMEM_ALLOC);
        return -1;
    }
}

void freeArray(dynamicArray* a) {
    if (a->array != NULL) {
        free(a->array);
        *a = createEmptyDynamicArray();
    } else {
        errcset(EFREE_NULLPTR);
    }
}

size_t arrayInsert(dynamicArray* a, Data item) {
    if (a == NULL || a->array == NULL) {
        errcset(ENULL_ARG);
        return -1;
    }
    if (a->used == a->size) { // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        Data* temp;
        temp = (Data*)realloc(a->array, sizeof(Data)*a->size);
        if (temp != NULL) {
            a->array = temp;
            return a->used;
        } else {
            errcset(EMEM_IREALLOC);
            return -1;
        }
    }
    a->used += 1;
    a->array[a->used - 1] = item;
    return a->used;
}

size_t arrayRemoveLast(dynamicArray* a) {
    if (a->used != 0) {
        a->used -= 1;
    } else {
        errcset(EARR_EMPTY);
        return -1;
    }
    MEM m = memoryDecrease(a);
    if (m =! NMEM_DECREASE) {
        return a->used;
    } else {
        return -1;
    }
}

size_t arrayRemoveItem(dynamicArray* a, Data item) {
    for (int i = 0; i < a->used; i++) {
        if (compareData(item, a->array[i]) == 0) { // compare with user defined compareData function
            return arrayRemoveAt(a, i);
        }
    }
    errcset(EDATA_NEXIST);
    return -1;
}

int arrayRemoveAt(dynamicArray* a, int index) {
    if (index > a->used) {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return -1;
    }
    for (int i = index; i < a->used; i++) {
        a->array[i] = a->array[i + 1];
    }
    a->used -= 1;
    MEM m = memoryDecrease(a);
    if (m =! NMEM_DECREASE) {
        return index;
    } else {
        return -1;
    }
}

MEM memoryDecrease(dynamicArray* a) {
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if(ratio <= QUARTER && a->size != 1) {
        a->size /= 2;
        Data* temp;
        temp = (Data*)realloc(a->array, sizeof(Data)*a->size);
        if (temp != NULL) {
            a->array = temp;
            return MEM_HALVED;
        } else {
            errcset(EMEM_DREALLOC);
            return NMEM_DECREASE;
        }
    }
    return MEM_MS_REACHED;
} 

size_t convert(Data b[], dynamicArray* a, size_t size) {
    *a = createEmptyDynamicArray();
    if(initDynamicArray(a, size) != size) {
        return -1;
    } 
    for (int i = 0; i < size; i++) {
        arrayInsert(a, b[i]);
    }
    return size;
}

size_t arrayUnion(dynamicArray* a, dynamicArray* b) {
    if (a == NULL || b == NULL) {
        errcset(ENULL_ARG);
        return -1;
    }
    for (int i = 0; i < b->used; i++) {
        arrayInsert(a, b->array[i]);
    }
    freeArray(b);
    b->array = NULL;
    return a->size;
}

size_t arrayClear(array* a) {
    if(a == NULL) {
        errcset(EARR_EMPTY);
        return -1;
    }
    int amount = a->used;
    for (int i = 0; i < amount; i++) {
        arrayRemoveLast(a);
    }
    return amount;
}