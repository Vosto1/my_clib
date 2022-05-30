#include "manual_dynamic_array.h"

mdynamicArray mcreateEmptyDynamicArray() {
    mdynamicArray a;
    a.array = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

size_t minitDynamicArray(mdynamicArray* a, size_t initSize) {
    Data* temp;
    temp = (Data*)malloc(sizeof(Data)*initSize);
    if (temp != NULL) {
        a->array = temp;
        a->size = initSize;
        marrayInitialize(a);
        return initSize;
    } else {
        a->array = NULL;
        errcset(EMEM_ALLOC);
        return -1;
    }
}

void mfreeArray(mdynamicArray* a) {
    if (a->array != NULL) {
        free(a->array);
        *a = mcreateEmptyDynamicArray();
    } else {
        errcset(EFREE_NULLPTR);
    }
}

void marrayInitialize(marray *a) {
    Data data = createPlaceholderData();
    for (int i = 0; i < a->size; i++) {
        a->array[i] = data;
    }
    a->used = 0;
}

int marrayInsert(mdynamicArray* a, Data item, int index) {
    if (a == NULL || a->array == NULL) {
        errcset(ENULL_ARG);
        return -1;
    }
    Data placeholder = createPlaceholderData();
    if (index < a->size) {
        if (compareData(a->array[index], placeholder) == 0) {
            a->array[index] = item;
            a->used += 1;
            return index;
        } else {
            errcset(EARR_INDEX_OCCUPIED);
            return -1;
        }
    } else {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return -1;
    }
}

int marrayRemoveAt(mdynamicArray* a, int index) {
    Data placeholder = createPlaceholderData();
    if (index >= a->size) {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return -1;
    } else if (compareData(a->array[index], placeholder) == 0) {
        return index;
    }
    a->array[index] = createPlaceholderData();
    a->used -= 1;
    return index;
}

int marrayCopy(mdynamicArray* copy, mdynamicArray* out) {
    *out = mcreateEmptyDynamicArray();
    if(minitDynamicArray(out, copy->size) != copy->size) {
        return -1;
    }
    for (int i = 0; i < copy->size; i++) {
        marrayInsert(out, copy->array[i], i);
    }
    return out->size;
}

bool mcheckMemoryIncrease(mdynamicArray* a) {
    return a->used == a->size;
}

bool mcheckMemoryDecrease(mdynamicArray* a) {
    if (a->size == 1) return false;
    double ratio = (double)a->used / (double)a->size;
    return ratio <= QUARTER;
}

MEM mmemoryDecrease(mdynamicArray* a) {
    // halve allocated space
    if(a->size != 1) {
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

MEM mmemoryIncrease(mdynamicArray* a) {
    a->size *= 2;
    Data* temp;
    Data placeholder = createPlaceholderData();
    temp = (Data*)malloc(sizeof(Data)*a->size);
    if (temp != NULL) {
        for (int i = 0; i < a->size; i++) {
            temp[i] = placeholder;
        }
        for (int i = 0; i < a->size / 2; i++) {
            temp[i] = a->array[i];
        }
        free(a->array);
        a->array = temp;
        return MEM_DOUBLED;
    } else { 
        errcset(EMEM_IREALLOC);
        return NMEM_INCREASE;
    }
}