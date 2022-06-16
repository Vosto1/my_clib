#include "hashtable.h"

hashtable ht_create_empty() {
    hashtable ht;
    ht.entries = NULL;
    ht.hash = NULL;
    ht.compare = NULL;
    ht.size = 0;
    return ht;
}

size_t ht_init(hashtable* ht, size_t size, size_t (*hash)(cvoidp,const hashtable*), int (*compare)(cvoidp,cvoidp)) {
    ht->entries = (voidp)malloc(sizeof(voidp) * size);
    if(!ht->entries) {
        errcset(EMEM_ALLOC);
        return -1;
    }
    ht->size = size;
    for (size_t i = 0; i < ht->size; i++) {
        ht->entries[i] = UNUSED;
    }
    ht->hash = hash;
    ht->compare = compare;
    return ht->size;
}

void ht_free(hashtable* ht) {
    if (ht == NULL) {
        errcset(EHASH_NULL);
    } else {
        for (size_t i = 0; i < ht_size(ht); i++) {
            if (ht->entries[i] != UNUSED) {
                free(ht->entries[i]);
            }
        }
        free(ht->entries);
        ht->entries = NULL;
        ht->compare = NULL;
        ht->hash = NULL;
        ht->size = 0;
    }
}

// only free the hashtable not the entries
void ht_destroy(hashtable* ht) {
    if (ht == NULL) {
        errcset(EHASH_NULL);
    } else {
        free(ht->entries);
        ht->entries = NULL;
        ht->compare = NULL;
        ht->hash = NULL;
        ht->size = 0;
    }
}

size_t ht_trim(hashtable* ht) {
    size_t size = ht_size(ht);
    s_array a = sda_create_empty();
    if (sda_init(&a, 1) != 1) {
        errcset(EHASH_TRIM_BUFFER);
        return -1;
    }
    
    // move all elements from the hashtable to an array
    for (size_t i = 0; i < size; i++)
        if (ht->entries[i] != UNUSED)
            sda_insert(&a, ht->entries[i]);
    
    // trim memory
    size_t elementCount = sda_count(&a);
    voidp* temp = (voidp*)realloc(ht->entries, elementCount * sizeof(voidp));
    if (temp == NULL) {
        errcset(EHASH_TRIM_MEMALLOC);
        return -1;
    }
    ht->entries = temp;
    ht->size = elementCount;

    // initialize ht
    for (size_t i = 0; i < elementCount; i++)
        ht->entries[i] = UNUSED;

    // re-insert elements
    voidp e;
    for (size_t i = 0; i < elementCount; i++) {
        e = sda_remove_last(&a);
        ht_insert(ht, e);
    }

    return ht->size;
}

size_t ht_insert(hashtable* ht, voidp element) {
    size_t index = linear_probe(ht, element);
    voidp e = ht->entries[index];
    if ((*ht->compare)(element, e) == 0) {
        // the value was set at index
        return index;
    } else {
        // fatal error (hashtable overflow)
        return -1;
    }
}

voidp ht_delete(hashtable* ht, cvoidp element) {
    if (ht_size(ht) > 0 && ht_lookup(ht, element) != UNUSED) {
        size_t index = indexof(ht, element);
        // save deleted element for later and unoccupy its' former slot
        voidp del = ht->entries[index];
        ht->entries[index] = UNUSED;

        // re-insert entries that might have been inserted with linear probing before
        voidp temp;
        size_t size = ht_size(ht);
        size_t i = 1;
        size_t entry = (index + i) % size;
        while (i < ht_size(ht) && ht->entries[entry] != UNUSED) {
            temp = ht->entries[entry];
            ht->entries[entry] = UNUSED;
            ht_insert(ht, temp);
            i++;
            entry = (index + i) % size;
        }
        // return the deleted element
        return del;
    } else return UNUSED;
}

// look up element, returns null if not present in table
voidp ht_lookup(const hashtable* ht, cvoidp element) {
    size_t size = ht_size(ht);
    size_t index = 0;
    size_t i = 0;
    size_t hash = (*ht->hash)(element, ht);
    voidp e;
    for (size_t i = 0; i < size; i++) {
        index = (hash + i) % size;  // modulu size to "wrap around"
        e = ht->entries[index];
        if (e == UNUSED) {
            return UNUSED; // element not in table
        } else if ((*ht->compare)(element, e) == 0) {
            return e; // found element at index: [index]
        }
    }
    return UNUSED; // element not in table (no free slots from start index to table end)
}


size_t ht_size(const hashtable* ht) {
    return ht->size;
}

size_t ht_count(const hashtable* ht) {
    size_t count = 0;
    for (size_t i = 0; i < ht_size(ht); i++) {
        if (ht->entries[i] != UNUSED)
            count++;
    }
    return count;
}

// get index of an element in the table
static size_t indexof(hashtable* ht, cvoidp valueToSearchFor)
{
    size_t size = ht_size(ht);
    size_t index = 0;
    size_t i = 0;
    size_t hash = (*ht->hash)(valueToSearchFor, ht);
    voidp e;
    for (size_t i = 0; i < size; i++) {
        index = (hash + i) % size;
        e = ht->entries[index];
        if ((*ht->compare)(valueToSearchFor, e) == 0) {
            return index; // found element at index: [index]
        }
    }
    // this function should only be called if
    // we know that the element is present in the table
    errcset(EHASHDATA_DOESNT_EXIST);
    return -1;
}

// try find slot that is empty or has the same key and set
// slot to new element of update the value of a present key
// if failure then: hashtable overflow, double ht size
static size_t linear_probe(hashtable* ht, voidp element) {
    size_t size = ht_size(ht);
    size_t index;
    size_t hash = (*ht->hash)(element, ht);
    for (size_t i = 0; i < size; i++) {
        index = (hash + i) % size; // modulu size to "wrap around"
        cvoidp e = ht->entries[index];
        if (e == UNUSED || (*ht->compare)(e, element) == 0) {
            ht->entries[index] = element; // update existing element or put element in an unoccupied slot
            return index;
        }
    }

    // hashtable overflow
    s_array a = sda_create_empty();
    if (sda_init(&a, ht_size(ht)) != ht_size(ht)) {
        errcset(EHASHTABLE_OVERFLOW_BUFFER);
        return -1;
    }

    // move all elements from the hashtable to an array
    for (size_t i = 0; i < ht_size(ht); i++) {
        if(ht->entries[i] != UNUSED) {
            voidp e = ht->entries[i];
            sda_insert(&a, e);
        }
    }

    // reallocate hashtable memory (double size)
    if (ht_size(ht) != 0)
        ht->size *= 2;
    else
        ht->size = 1;
    
    voidp* temp = (voidp*)realloc(ht->entries, ht->size * sizeof(voidp));
    if (temp == NULL) {
        errcset(EHASHTABLE_OVERFLOW_MEMALLOC);
        return -1; // fatal error
    }
    ht->entries = temp;

    // initialize to UNUSED
    for (size_t i = 0; i < ht_size(ht); i++) {
        ht->entries[i] = UNUSED;
    }

    // re-insert all elements in the array into the new ht
    size_t elements = sda_count(&a);
    for (size_t i = 0; i < elements; i++) {
        voidp e = sda_remove_last(&a);
        ht_insert(ht, e);
    }

    // insert the element that we wanted to add from the beginning
    return ht_insert(ht, element);
}