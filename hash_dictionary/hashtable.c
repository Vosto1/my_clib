#include "hashtable.h"

static int indexof(hashtable *ht, const void* value_to_search_for);
static int linear_probe(hashtable *ht, void* element, int* collisions);

hashtable ht_create_empty()
{
    hashtable ht;
    ht.entries = NULL;
    ht.hash = NULL;
    ht.compare = NULL;
    ht.size = 0;
    return ht;
}

int ht_init(hashtable *ht, int size, uint (*hash)(const void*, const hashtable *), int (*compare)(const void*, const void*), void (*freeObject)(void*))
{
    ht->entries = (void*)malloc(sizeof(void*) * size);
    if (!ht->entries)
    {
        return ERRMEM;
    }
    ht->size = size;
    for (int i = 0; i < ht->size; i++)
    {
        ht->entries[i] = UNUSED;
    }
    ht->hash = hash;
    ht->compare = compare;
    ht->freeObject = freeObject;
    return ht->size;
}

bool ht_free(hashtable *ht)
{
    if (ht == NULL)
    {
        return false;
    }
    else
    {
        for (int i = 0; i < ht_size(ht); i++)
        {
            if (ht->entries[i] != UNUSED)
                (*ht->freeObject)(ht->entries[i]);
        }
        free(ht->entries);
        ht->entries = NULL;
        ht->compare = NULL;
        ht->hash = NULL;
        ht->size = 0;
        return true;
    }
}

// only free the hashtable not the entries
bool ht_destroy(hashtable *ht)
{
    if (ht == NULL)
        return false;
        //errcset(EHASH_NULL);
    else
    {
        free(ht->entries);
        ht->entries = NULL;
        ht->compare = NULL;
        ht->hash = NULL;
        ht->size = 0;
        return true;
    }
}

int ht_trim(hashtable *ht)
{
    darray a = ht_to_array(ht);
    // trim memory
    int elementCount = da_count(&a);
    void* *temp = (void* *)realloc(ht->entries, elementCount * sizeof(void*));
    if (temp == NULL)
    {
        return ERRMEMR;
    }
    ht->entries = temp;
    ht->size = elementCount;

    // initialize ht
    for (int i = 0; i < elementCount; i++)
        ht->entries[i] = UNUSED;

    // re-insert elements
    void* e;
    for (int i = 0; i < elementCount; i++)
    {
        e = da_remove_last(&a);
        ht_insert(ht, e);
    }

    // free darray
    da_destroy(&a);

    return ht->size;
}

darray ht_to_array(hashtable* ht)
{
    int size = ht_size(ht);
    darray a = da_create_empty();
    if (da_init(&a, size, ht->compare, ht->freeObject) != size)
    {
        return da_create_empty();
    }

    // move all elements from the hashtable to an array
    for (int i = 0; i < size; i++)
        if (ht->entries[i] != UNUSED)
            da_insert(&a, ht->entries[i]);
    return a;
}

hashtable ht_from_array(darray* a, uint (*hash)(const void*, const hashtable *))
{
    assert(a->compare != NULL);
    assert(a->freeObject != NULL);
    hashtable ht = ht_create_empty();
    int count = da_count(a);
    assert(ht_init(&ht, count, hash, a->compare, a->freeObject) == count);
    for (int i = 0; i < count; i++)
    {
        ht_insert(&ht, da_remove_last(a));
    }
    return ht;
}

// returns collisions if insert was successful, otherwise -1
int ht_insert(hashtable *ht, void* element)
{
    int collisions = 0;
    int index = linear_probe(ht, element, &collisions);
    if (index > ht_size(ht))
        return -1;
    else
        return collisions;
}

void* ht_delete(hashtable *ht, const void* element)
{
    if (ht_size(ht) > 0 && ht_lookup(ht, element) != UNUSED)
    {
        int index = indexof(ht, element);
        // save deleted element for later and unoccupy its' former slot
        void* del = ht->entries[index];
        ht->entries[index] = UNUSED;

        // re-insert entries that might have been inserted with linear probing before
        void* temp;
        int size = ht_size(ht);
        int i = 1;
        int entry = (index + i) % size;
        while (i < ht_size(ht) && ht->entries[entry] != UNUSED)
        {
            temp = ht->entries[entry];
            ht->entries[entry] = UNUSED;
            ht_insert(ht, temp);
            i++;
            entry = (index + i) % size;
        }
        // return the deleted element
        return del;
    }
    else
        return UNUSED;
}

// look up element, returns null if not present in table
void* ht_lookup(const hashtable *ht, const void* element)
{
    int size = ht_size(ht);
    int index = 0;
    int hash = (*ht->hash)(element, ht);
    void* e;
    for (int i = 0; i < size; i++)
    {
        index = (hash + i) % size; // modulu size to "wrap around"
        e = ht->entries[index];
        if (e == UNUSED)
        {
            return UNUSED; // element not in table
        }
        else if ((*ht->compare)(element, e) == 0)
        {
            return e; // found element at index: [index]
        }
    }
    return UNUSED; // element not in table (no free slots from start index to table end)
}

int ht_size(const hashtable *ht)
{
    return ht->size;
}

int ht_count(const hashtable *ht)
{
    int count = 0;
    int size = ht_size(ht);
    for (int i = 0; i < size; i++)
    {
        if (ht->entries[i] != UNUSED)
            count++;
    }
    return count;
}


// get index of an element in the table
// returns a value bigger than the size of the htable if the element didnt exist
static int indexof(hashtable *ht, const void* value_to_search_for)
{
    int size = ht_size(ht);
    int index = 0;
    int hash = (*ht->hash)(value_to_search_for, ht);
    void* e;
    for (int i = 0; i < size; i++)
    {
        index = (hash + i) % size;
        e = ht->entries[index];
        if ((*ht->compare)(value_to_search_for, e) == 0)
        {
            return index; // found element at index: [index]
        }
    }
    // this function should only be called if
    // we know that the element is present in the table
    // so we shouldnt get here
    return INVALIDINDEX;
}

// try find slot that is empty or has the same key and set
// slot to new element of update the value of a present key
// if failure then: hashtable overflow, double ht size
// returns amount of collisions by reference, and index where the element was put by value
static int linear_probe(hashtable *ht, void* element, int* collisions)
{
    int size = ht_size(ht);
    int index;
    int hash = (*ht->hash)(element, ht);
    for (int i = 0; i < size; i++)
    {
        index = (hash + i) % size; // modulu size to "wrap around"
        const void* e = ht->entries[index];
        if (e == UNUSED || (*ht->compare)(e, element) == 0)
        {
            if ( e != UNUSED && (*ht->compare)(e, element) == 0)
            {
                // remove old value (that was equal)
                // so that it can be updated 
                // this makes sure there is no leaked memory
                (*ht->freeObject)((void*)e);
            }
            ht->entries[index] = element; // update existing element or put element in an unoccupied slot
            *collisions =  (int)i; // i == collisions
            return index;
        }
    }

    // hashtable overflow
    darray a = da_create_empty();
    int errcode;
    if ((errcode = da_init(&a, ht_size(ht), ht->compare, ht->freeObject)) != ht_size(ht))
    {
        return errcode;
    }

    // move all elements from the hashtable to an array
    for (int i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            void* e = ht->entries[i];
            da_insert(&a, e);
        }
    }

    // reallocate hashtable memory (double size)
    if (ht_size(ht) != 0)
        ht->size *= 2;
    else
        ht->size = 1;

    void* *temp = (void* *)realloc(ht->entries, ht->size * sizeof(void*));
    if (temp == NULL)
    {
        return ERRMEMR;
    }
    ht->entries = temp;

    // initialize to UNUSED
    for (int i = 0; i < ht_size(ht); i++)
    {
        ht->entries[i] = UNUSED;
    }

    // re-insert all elements in the array into the new ht
    int elements = da_count(&a);
    for (int i = 0; i < elements; i++)
    {
        void* e = da_remove_last(&a);
        ht_insert(ht, e);
    }

    // free darray
    da_destroy(&a);

    // insert the element that we wanted to add from the beginning
    return ht_insert(ht, element);
}