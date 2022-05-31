#include "dynamic_array.h"

dynamicArray createEmptyDynamicArray() {
    dynamicArray a;
    a.array = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

ErrorCode initDynamicArray(dynamicArray* a, size_t initSize) {
    a->size = 0;
    a->used = 0;
    Data* temp;
    temp = (Data*)malloc(sizeof(Data)*initSize);
    if (temp != NULL) {
        a->array = temp;
        a->size = initSize;
        return SUCCESS;
    } else { a->array = NULL; return MEM_ALLOC; }
}

ErrorCode freeArray(dynamicArray* a) {
    if (a->array != NULL) {
        free(a->array);
        a->array = NULL;
        a->size = 0;
        a->used = 0;
    } else return FREE_NULLPTR;
}

ErrorCode arrayInsert(dynamicArray* a, Data item) {
    if (a->used == a->size) { // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        Data* temp;
        temp = (Data*)realloc(a->array, sizeof(Data)*a->size);
        if (temp != NULL) {
            a->array = temp;
        } else return MEM_IREALLOC;
    }
    a->used += 1;
    a->array[a->used - 1] = item;
    return SUCCESS;
}

ErrorCode memoryDecrease(dynamicArray* a) {
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if(ratio <= QUARTER && a->size != 1) {
        a->size /= 2;
        Data* temp;
        temp = (Data*)realloc(a->array, sizeof(Data)*a->size);
        if (temp != NULL) {
            a->array = temp;
        } else return MEM_DREALLOC;
    }
    return SUCCESS;
} 

ErrorCode arrayRemoveLast(dynamicArray* a) {
    if (a->used != 0)
        a->used -= 1;
    else return ARR_EMPTY;
    return memoryDecrease(a);
}

ErrorCode arrayRemoveItem(dynamicArray* a, Data item) {
    for (int i = 0; i < a->used; i++) {
        if (compareData(item, a->array[i]) == 0) { // arrayCompare with user defined arrayCompare function
            return arrayRemoveAt(a, i);
        }
    }
    return DATA_EXIST;
}

ErrorCode arrayRemoveAt(dynamicArray* a, int index) {
    if (index > a->used) {
        return INDEX_OUT_OF_BOUNDS;
    }
    for (int i = index; i < a->used; i++) {
        a->array[i] = a->array[i + 1];
    }
    a->used -= 1;
    return memoryDecrease(a);
}

ErrorCode convert(Data b[], dynamicArray* a, size_t size) {
    *a = createEmptyDynamicArray();
    initDynamicArray(a, size);
    for (int i = 0; i < size; i++) {
        arrayInsert(a, b[i]);
    }
    return SUCCESS;
}

ErrorCode arrayUnion(dynamicArray* a, dynamicArray* b) {
    if (a == NULL || b == NULL) {
        return NULL_ARG;
    }
    for (int i = 0; i < b->used; i++) {
        arrayInsert(a, b->array[i]);
    }
    freeArray(b);
    b->array = NULL;
    return SUCCESS;
}

ErrorCode arrayClear(array* a) {
    if(a == NULL) return ARR_EMPTY;
    int amount = a->used;
    for (int i = 0; i < amount; i++) {
        arrayRemoveLast(a);
    }
    return SUCCESS;
}