#include "dynamic_array.h"

dynamicArray createEmptyDynamicArray() {
    dynamicArray a;
    a.array = NULL;
    a.compare = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

size_t initDynamicArray(dynamicArray* a, size_t initSize, int (*compare)(Data a, Data b)) {
    a->size = 0;
    a->used = 0;
    Data* temp;
    temp = (Data*)malloc(sizeof(Data)*initSize);
    if (temp != NULL) {
        a->array = temp;
        a->size = initSize;
        a->compare = compare;
        return initSize;
    } else {
        *a = createEmptyDynamicArray();
        errcset(EMEM_ALLOC);
        return -1;
    }
}

void freeArray(dynamicArray* a) {
    if (!a_is_null(a)) {
        if (!a_is_empty(a))
            arrayClear(a);
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
        } else {
            errcset(EMEM_IREALLOC);
            return -1;
        }
    }
    a->used += 1;
    a->array[a->used - 1] = item;
    return a->used;
}

Data arrayRemoveLast(dynamicArray* a) {
    if (a_is_empty(a)) {
        errcset(EARR_EMPTY);
        return NULL;
    }
    a->used -= 1;
    MEM m = memoryDecrease(a);
    if (m != NMEM_DECREASE) {
        Data data = a->array[a->used];
        return data;
    } else {
        a->used += 1; // rollback
        return NULL;
    }
}

Data arrayRemoveItem(dynamicArray* a, Data item) {
    for (int i = 0; i < a->used; i++) {
        if ((*a->compare)(item, a->array[i]) == 0) { // compare with user defined function
            return arrayRemoveAt(a, i);
        }
    }
    errcset(EDATA_NEXIST);
    return NULL;
}

Data arrayRemoveAt(dynamicArray* a, int index) {
    if (index > a->used) {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    Data* data = a->array[index];
    for (int i = index; i < a->used; i++) {
        a->array[i] = a->array[i + 1];
    }
    a->used -= 1;
    MEM m = memoryDecrease(a);
    if (m != NMEM_DECREASE) {
        return data;
    } else {
        a->used += 1; // rollback
        a->array[a->used] = data; // rollback
        return NULL;
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
    return MEM_MS_REACHED_NO_NEED;
} 

size_t convert(dynamicArray* a, Data b[], size_t bsize, int (*compare)(Data a, Data b)) {
    *a = createEmptyDynamicArray();
    if(initDynamicArray(a, bsize, compare) != bsize) {
        return -1;
    } 
    for (int i = 0; i < bsize; i++) {
        arrayInsert(a, b[i]);
    }
    return bsize;
}

size_t arrayUnion(dynamicArray* a, dynamicArray* b) {
    if (a == NULL || b == NULL) {
        errcset(ENULL_ARG);
        return -1;
    }
    for (int i = 0; i < b->used; i++) {
        arrayInsert(a, b->array[i]);
    }
    // free only array not the elements (freeArray frees all items in the array as well)
    free(b->array);
    b->size = 0;
    b->used = 0;
    b->array = NULL;
    return a->size;
}

size_t arrayClear(array* a) {
    if(a_is_empty(a)) {
        errcset(EARR_EMPTY);
        return -1;
    } else if (a_is_null(a)) {
        errcset(ENULL_ARG);
        return -1;
    }
    int amount = a->used;
    Data d;
    for (int i = 0; i < amount; i++) {
        d = arrayRemoveLast(a);
        free(d);
    }
    return amount;
}

bool a_is_null(array* a) {
    return a->array == NULL;
}

bool a_is_empty(array* a) {
    return a->used == 0;
}