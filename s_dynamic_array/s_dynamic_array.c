#include "s_dynamic_array.h"

size_t sda_count(s_array* a) {
    return a->used;
}

size_t sda_size(s_array* a) {
    return a->size;
}

s_dynamicArray sda_create_empty() {
    s_dynamicArray a;
    a.array = NULL;
    a.size = 0;
    a.used = 0;
    return a;
}

size_t sda_init(s_dynamicArray* a, size_t initSize) {
    a->size = 0;
    a->used = 0;
    voidp* temp;
    temp = (voidp*)malloc(sizeof(voidp)*initSize);
    if (temp != NULL) {
        a->array = temp;
        a->size = initSize;
        return initSize;
    } else {
        *a = sda_create_empty();
        errcset(EMEM_ALLOC);
        return -1;
    }
}

size_t sda_clear(s_array* a) {
    if(sda_is_empty(a)) {
        errcset(EARR_EMPTY);
        return -1;
    } else if (sda_is_null(a)) {
        errcset(ENULL_ARG);
        return -1;
    }
    int amount = a->used;
    voidp d;
    for (int i = 0; i < amount; i++) {
        d = sda_remove_last(a);
        free(d);
    }
    return amount;
}

void sda_destroy(s_dynamicArray* a) {
    if (!sda_is_null(a)) {
        free(a->array);
        *a = sda_create_empty();
        a->size = 0;
        a->used = 0;
        a->array = NULL;
    } else {
        errcset(EFREE_NULLPTR);
    }
}

void sda_free(s_dynamicArray* a) {
    if (!sda_is_null(a)) {
        if (!sda_is_empty(a)) {
            sda_clear(a);
            sda_destroy(a);
        }
    } else {
        errcset(EFREE_NULLPTR);
    }
}

size_t sda_insert(s_dynamicArray* a, voidp item) {
    if (a == NULL || a->array == NULL) {
        errcset(ENULL_ARG);
        return -1;
    }
    // memory increase
    if (a->used == a->size) { // index < size, is a guard
        // when array is full on insert, double the size
        a->size *= 2;
        voidp* temp;
        temp = (voidp*)realloc(a->array, sizeof(voidp)*a->size);
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

voidp sda_remove_last(s_dynamicArray* a) {
    if (sda_is_empty(a)) {
        errcset(EARR_EMPTY);
        return NULL;
    }
    a->used -= 1;
    MEM m = sda_memory_decrease(a);
    if (m != NMEM_DECREASE) {
        voidp data = a->array[a->used];
        return data;
    } else {
        a->used += 1; // rollback
        return NULL;
    }
}

voidp sda_remove_at(s_dynamicArray* a, int index) {
    if (index > a->used) {
        errcset(EINDEX_OUT_OF_BOUNDS);
        return NULL;
    }
    voidp* data = a->array[index];
    for (int i = index; i < a->used; i++) {
        a->array[i] = a->array[i + 1];
    }
    a->used -= 1;
    MEM m = sda_memory_decrease(a);
    if (m != NMEM_DECREASE) {
        return data;
    } else {
        a->used += 1; // rollback
        a->array[a->used] = data; // rollback
        return NULL;
    }
}

size_t sda_convert(s_dynamicArray* a, voidp b[], size_t bsize) {
    *a = sda_create_empty();
    if(sda_init(a, bsize) != bsize) {
        return -1;
    }
    for (int i = 0; i < bsize; i++) {
        sda_insert(a, b[i]);
    }
    return bsize;
}

size_t sda_merge(s_dynamicArray* a, s_dynamicArray* b) {
    if (a == NULL || b == NULL) {
        errcset(ENULL_ARG);
        return -1;
    }
    for (int i = 0; i < b->used; i++) {
        sda_insert(a, b->array[i]);
    }
    // free only array not the elements (sda_free frees all items in the array as well)
    sda_destroy(b);
    return a->size;
}

bool sda_is_null(s_array* a) {
    return a->array == NULL;
}

bool sda_is_empty(s_array* a) {
    return a->used == 0;
}

// memory check and increase is done in arrayInsert function
static MEM sda_memory_decrease(s_dynamicArray* a) {
    double ratio = (double)a->used / (double)a->size;
    // if 1/4 of the allocated space is used, halve it
    if(ratio <= QUARTER && sda_size(a) != 1) {
        a->size /= 2;
        voidp* temp;
        temp = (voidp*)realloc(a->array, sizeof(voidp)*a->size);
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
