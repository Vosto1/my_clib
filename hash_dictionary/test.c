#include "test.h"

static int compare(const void* e1, const void* e2)
{
    entry* en1 = (entry*)e1;
    entry* en2 = (entry*)e2;
    return (int)en1->k - (int)en2->k;
}

static uint hashfn(const void* e, const hashtable* ht)
{
    entry* f = (entry*)e;
    
    int index = f->k * 37;
    index %= ht_size(ht);
    if (index < 0)
    {
        index *= -1;
    }
    return index;
}

static entry* _create_entry(key k, value v)
{
    entry* e = (entry*)malloc(sizeof(entry));
    e->k = k;
    e->v = v;
    return e;
}

static void free_entry(void* vp)
{
    entry* e = (entry*)vp;
    free(e);
}

char rascii()
{
    return (char)(rand() % 94 + 32); // ascii character value span
}

static entry* randomElement(hashtable* ht)
{
    int size = ht_size(ht);
    int i = 0;
    for (; i < size; i++)
    {
        if (ht->entries[i] != UNUSED)
            return (entry*)ht->entries[i];
    }
    return UNUSED;
}

// debug
static void pvalue(entry* e, int index)
{
    printf("index %d: [%c,%d]\n", index, e->k, e->v);
}

static void print(hashtable* ht)
{
    for (int i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            entry* e = (entry*)ht->entries[i];
            pvalue(e, i);
        }
        else
            printf("index %d: UNUSED\n", i);
    }
    printf("\n");
}

uint auto_tests(int tests, int mod)
{
    srand(time(NULL));

    hashtable ht = ht_create_empty();
    ht_init(&ht, 10, &hashfn, &compare, &free_entry);
    uint count;
    #ifdef HT_VERBOSE
    ticks start, end, prgStart, prgEnd;
    uint deletions = 0, insertions = 0, lookups = 0, toarrays = 0, trims = 0;
    double avg_collisions;
    int collisions = 0;
    #endif
    uint operations = 0;
    int tmp_col;
    uint random, nexttests, val;
    entry* element;
    entry* e;
    entry* del; 
    bool existsht = false;

    
    #ifdef HT_VERBOSE
    prgStart = now();
    #endif
    for (uint i = 0; i < tests; i++)
    {
        nexttests = rand() % mod;
        #ifdef HT_VERBOSE
        start = now();
        #endif
        for (uint j = 0; j < nexttests; j++)
        {
            val = rand(); // random integer instead to get more collisions //rascii(); // random ascii character
            random = rand() % 100;
            if (random < 80)
            { // insert
                #ifdef HT_DEBUG
                printf("insertion, element count: %d\n", ht_count(&ht));
                #endif
                element = _create_entry((char)val, val);
                existsht = ht_lookup(&ht, element) != NULL; // check if element already exists in ht
                count = ht_count(&ht);
                // debug
                //print(&ht);
                tmp_col = ht_insert(&ht, element);
                assert(tmp_col != -1);
                // debug
                //print(&ht);
                if (!existsht) // if it already existed the element was "updated" (its not visible since its updated to the same values)
                {
                    int new_count = ht_count(&ht);
                    assert(new_count == count + 1);
                }
                assert(ht_lookup(&ht, element) != NULL); // check if the element was inserted
                #ifdef HT_VERBOSE
                collisions += tmp_col;
                insertions++;
                #endif
            }
            else if (random >= 80 && random < 90)
            { // delete
                if (ht_count(&ht) > 0)
                {
                    #ifdef HT_DEBUG
                    printf("deletion, element count: %d\n", ht_count(&ht));
                    #endif
                    e = randomElement(&ht);
                    // debug
                    //pvalue(e);
                    count = ht_count(&ht);
                    del = ht_delete(&ht, e);
                    assert(del != NULL); // make sure that the element was deleted (e cannot be a element that doesnt exist in ht)
                    assert(ht_count(&ht) == (count - 1));
                    e = ht_lookup(&ht, del);
                    assert(e == NULL); // del shouldnt be in ht
                    free(del);
                    #ifdef HT_VERBOSE
                    deletions++;
                    #endif
                }
            }
            else if (random >= 90 && random < 95)
            { // look up
                if (ht_count(&ht) > 0)
                {
                    #ifdef HT_DEBUG
                    printf("lookup, element count: %d\n", ht_count(&ht));
                    #endif
                    element = randomElement(&ht);
                    // debug
                    //pvalue(element);
                    //print(&ht);
                    e = ht_lookup(&ht, element);
                    assert(element == e); // make sure we found the right element
                    #ifdef HT_VERBOSE
                    lookups++;
                    #endif
                }
            }
            else if (random >= 95 && random < 97)
            {
                #ifdef HT_DEBUG
                printf("to array, element count: %d\n", ht_count(&ht));
                #endif
                sdarray a = ht_to_array(&ht);
                int size = sda_count(&a);
                for (int i = 0; i < size; i++)
                {
                    assert(ht_lookup(&ht, a.array[i]) != NULL);
                }
                sda_destroy(&a);
                #ifdef HT_VERBOSE
                toarrays++;
                #endif
            }
            else
            {
                #ifdef HT_DEBUG
                printf("trim, element count: %d\n", ht_count(&ht));
                #endif
                sdarray a = ht_to_array(&ht);
                int elementCount = sda_count(&a);
                if (elementCount != 0)
                {
                    ht_trim(&ht);
                    int size = ht_size(&ht);
                    assert(size == elementCount);
                    for (int i = 0; i < ht_size(&ht); i++)
                    {
                        assert(ht.entries[i] != UNUSED);
                    }
                    #ifdef HT_VERBOSE
                    trims++;
                    #endif
                }
                sda_destroy(&a);
            }
            operations++;
        }
        #ifdef HT_VERBOSE
        end = now();
        seconds subtest = diff(start, end);
        if (insertions != 0)
            avg_collisions = (double)collisions / (double)insertions;
        else
            avg_collisions = 0.0f;
        printf("Computed %d operations (%d insertions with %d collisions and %.1f average collisions, %d deletions %d lookups %d to array conversions %d trims) during %f\n", nexttests, insertions, collisions, avg_collisions, deletions, lookups, toarrays, trims, subtest);
        insertions = 0;
        deletions = 0;
        lookups = 0;
        toarrays = 0;
        trims = 0;
        collisions = 0;
        #endif
    }

    // free
    assert(ht_free(&ht));


    #ifdef HT_VERBOSE
    prgEnd = now();
    seconds programTime = diff(prgStart, prgEnd);
    printf("Test completed, computed %d operations during %fs\n", operations, programTime);
    #endif
    return operations;
}

void test_sequence()
{
    entry* del;
    hashtable ht = ht_create_empty();
    ht_init(&ht, 3, &hashfn, &compare, &free_entry);
    entry* e1 = _create_entry('c', 1);
    entry* e2 = _create_entry('/', 3);
    entry* e3 = _create_entry('#', 7);
    entry* e4 = _create_entry('<', 5);

    assert(ht_delete(&ht, e1) == UNUSED);
    assert(ht_lookup(&ht, e1) == UNUSED);

    ht_insert(&ht, e1);
    ht_insert(&ht, e2);
    ht_insert(&ht, e3);
    print(&ht);
    del = ht_delete(&ht, e2);
    assert(ht_size(&ht) == 3);
    assert(del != NULL);

    assert(ht_lookup(&ht, e2) == NULL);
    print(&ht);
    assert(ht_lookup(&ht, e1) != UNUSED && ht_lookup(&ht, e3) != UNUSED);
    ht_insert(&ht, e4);
    print(&ht);

    ht_delete(&ht, e4);
    assert(ht_lookup(&ht, e4) == NULL);
    print(&ht);

    ht_delete(&ht, e3);
    assert(ht_lookup(&ht, e3) == NULL);
    print(&ht);

    ht_delete(&ht, e1);
    assert(ht_lookup(&ht, e1) == NULL);
    print(&ht);
    printf("-----------------\n");
    ht_insert(&ht, e1);
    print(&ht);
    ht_insert(&ht, e2);
    print(&ht);
    ht_insert(&ht, e3);
    print(&ht);
    ht_insert(&ht, e4);
    print(&ht);

    assert(ht_free(&ht));
    printf("Test passed\n");
}