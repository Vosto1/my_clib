#include "hashtable.h"

static size_t indexof(hashtable *ht, const void* value_to_search_for);
static size_t linear_probe(hashtable *ht, void* element, int* collisions);

hashtable ht_create_empty()
{
    hashtable ht;
    ht.entries = NULL;
    ht.hash = NULL;
    ht.compare = NULL;
    ht.size = 0;
    return ht;
}

size_t ht_init(hashtable *ht, size_t size, size_t (*hash)(const void*, const hashtable *), int (*compare)(const void*, const void*), void (*freeObject)(void*))
{
    ht->entries = (void*)malloc(sizeof(void*) * size);
    if (!ht->entries)
    {
        //errcset(EMEM_ALLOC);
        return 0;
    }
    ht->size = size;
    for (size_t i = 0; i < ht->size; i++)
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
        //errcset(EHASH_NULL);
        return false;
    }
    else
    {
        for (size_t i = 0; i < ht_size(ht); i++)
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

size_t ht_trim(hashtable *ht)
{
    size_t size = ht_size(ht);
    sdarray a = sda_create_empty();
    if (sda_init(&a, size, NULL) != size)
    {
        //errcset(EHASH_TRIM_BUFFER);
        return 0;
    }

    // move all elements from the hashtable to an array
    for (size_t i = 0; i < size; i++)
        if (ht->entries[i] != UNUSED)
            sda_insert(&a, ht->entries[i]);

    // trim memory
    size_t elementCount = sda_count(&a);
    void* *temp = (void* *)realloc(ht->entries, elementCount * sizeof(void*));
    if (temp == NULL)
    {
        //errcset(EHASH_TRIM_MEMALLOC);
        return 0;
    }
    ht->entries = temp;
    ht->size = elementCount;

    // initialize ht
    for (size_t i = 0; i < elementCount; i++)
        ht->entries[i] = UNUSED;

    // re-insert elements
    void* e;
    for (size_t i = 0; i < elementCount; i++)
    {
        e = sda_remove_last(&a);
        ht_insert(ht, e);
    }

    // free sdarray
    sda_destroy(&a);

    return ht->size;
}

// returns collisions if insert was successful, otherwise -1
int ht_insert(hashtable *ht, void* element)
{
    int collisions = 0;
    size_t index = linear_probe(ht, element, &collisions);
    if (index > ht_size(ht))
        return -1;
    else
        return collisions;
}

void* ht_delete(hashtable *ht, const void* element)
{
    if (ht_size(ht) > 0 && ht_lookup(ht, element) != UNUSED)
    {
        size_t index = indexof(ht, element);
        // save deleted element for later and unoccupy its' former slot
        void* del = ht->entries[index];
        ht->entries[index] = UNUSED;

        // re-insert entries that might have been inserted with linear probing before
        void* temp;
        size_t size = ht_size(ht);
        size_t i = 1;
        size_t entry = (index + i) % size;
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
    size_t size = ht_size(ht);
    size_t index = 0;
    size_t hash = (*ht->hash)(element, ht);
    void* e;
    for (size_t i = 0; i < size; i++)
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

size_t ht_size(const hashtable *ht)
{
    return ht->size;
}

size_t ht_count(const hashtable *ht)
{
    size_t count = 0;
    for (size_t i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
            count++;
    }
    return count;
}

#define FLAG_INDEX_ERROR 1000

// get index of an element in the table
// returns a value bigger than the size of the htable if the element didnt exist
static size_t indexof(hashtable *ht, const void* value_to_search_for)
{
    size_t size = ht_size(ht);
    size_t index = 0;
    size_t hash = (*ht->hash)(value_to_search_for, ht);
    void* e;
    for (size_t i = 0; i < size; i++)
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
    //errcset(EHASHDATA_DOESNT_EXIST);
    return ht->size + FLAG_INDEX_ERROR;
}

// try find slot that is empty or has the same key and set
// slot to new element of update the value of a present key
// if failure then: hashtable overflow, double ht size
// returns amount of collisions by reference, and index where the element was put by value
static size_t linear_probe(hashtable *ht, void* element, int* collisions)
{
    size_t size = ht_size(ht);
    size_t index;
    size_t hash = (*ht->hash)(element, ht);
    for (size_t i = 0; i < size; i++)
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
                //free((void*)e);
                printf("fatal error\n");
                exit(-1);
            }
            ht->entries[index] = element; // update existing element or put element in an unoccupied slot
            *collisions =  (int)i; // i == collisions
            return index;
        }
    }

    // hashtable overflow
    sdarray a = sda_create_empty();
    if (sda_init(&a, ht_size(ht), NULL) != ht_size(ht))
    {
        //errcset(EHASHTABLE_OVERFLOW_BUFFER);
        return ht->size + FLAG_INDEX_ERROR;
    }

    // move all elements from the hashtable to an array
    for (size_t i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            void* e = ht->entries[i];
            sda_insert(&a, e);
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
        //errcset(EHASHTABLE_OVERFLOW_MEMALLOC);
        return ht->size + FLAG_INDEX_ERROR; // fatal error
    }
    ht->entries = temp;

    // initialize to UNUSED
    for (size_t i = 0; i < ht_size(ht); i++)
    {
        ht->entries[i] = UNUSED;
    }

    // re-insert all elements in the array into the new ht
    size_t elements = sda_count(&a);
    for (size_t i = 0; i < elements; i++)
    {
        void* e = sda_remove_last(&a);
        ht_insert(ht, e);
    }

    // free sdarray
    sda_destroy(&a);

    // insert the element that we wanted to add from the beginning
    return ht_insert(ht, element);
}