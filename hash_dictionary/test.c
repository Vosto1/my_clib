#include "test.h"

int compare(cvoidp e1, cvoidp e2) {
    entry* en1 = (entry*)e1;
    entry* en2 = (entry*)e2;
    return (int)en1->k - (int)en2->k;
}

size_t hashfn(cvoidp e, const hashtable* ht) {
    entry* f = (entry*)e;
    
    size_t index = f->k * 37;
    index %= ht_size(ht);
    return index;
}

entry* createEntry(key k, value v) {
    entry* e = (entry*)malloc(sizeof(entry));
    e->k = k;
    e->v = v;
    return e;
}

entry* randomElement(hashtable* ht) {
    size_t size = ht_size(ht);
    size_t i = 0;
    for (; i < size; i++) {
        if (ht->entries[i] != UNUSED)
            return (entry*)ht->entries[i];
    }
    return UNUSED;
}

// better random concept but not working (too lazy to fix rn)
/* entry* randomElement(hashtable* ht) {
    size_t size = ht_size(ht);
    size_t startIndex = rand() % ht_size(ht);
    size_t i = startIndex;
    size_t index = 0;
    for (; i < size + startIndex; i++) {
        index = i % size; // "wrap around"
        if (ht->entries[index] != UNUSED)
            return (entry*)ht->entries[i];
    }
    return UNUSED;
} */

// debug
void print(hashtable* ht) {
    for (int i = 0; i < ht_size(ht); i++) {
        if (ht->entries[i] != UNUSED) {
            entry* e = (entry*)ht->entries[i];
            printf("index %d: [%c,%d]\n", i, e->k, e->v);
        } else printf("index %d: UNUSED\n", i);
    }
    printf("\n");
}

void pvalue(entry* e) {
    printf("key: %c value: %d\n", e->k, e->v);
}


unsigned int auto_tests(int tests, int mod) {
    srand(time(NULL));

    hashtable ht = ht_create_empty();
    ht_init(&ht, 10, &hashfn, &compare);
    ticks start, end, prgStart, prgEnd;
    unsigned int count;
    unsigned int operations = 0, deletions = 0, insertions = 0, lookups = 0;
    unsigned int random, nexttests, val;
    entry* element;
    entry* e;
    entry* del; 
    bool existsht = false;

    prgStart = now();
    for (unsigned int i = 0; i < tests; i++) {
        nexttests = rand() % mod;
        start = now();
        for (unsigned int j = 0; j < nexttests; j++) {
            val = rand() % 94 + 32; // ascii character value span
            random = rand() % 100;
            if (random < 80) { // insert
                //printf("insertion, element count: %d\n", ht_count(&ht));
                element = createEntry((char)val, val);
                existsht = ht_lookup(&ht, element) != NULL; // check if element already exists in ht
                count = ht_count(&ht);
                // debug
                //print(&ht);
                assert(ht_insert(&ht, element) != -1);
                // debug
                //print(&ht);
                if (!existsht) // if it already existed the element was "updated" (its not visible since its updated to the same values)
                    assert(ht_count(&ht) == count + 1);
                assert(ht_lookup(&ht, element) != NULL); // check if the element was inserted
                insertions++;
            }
            else if (random > 80 && random < 90) { // delete
                if (ht_count(&ht) > 0) {
                    //printf("deletion, element count: %d\n", ht_count(&ht));
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
                    deletions++;
                }
            }
            else { // look up
                if (ht_count(&ht) > 0) {
                    //printf("lookup, element count: %d\n", ht_count(&ht));
                    element = randomElement(&ht);
                    // debug
                    //pvalue(element);
                    //print(&ht);
                    e = ht_lookup(&ht, element);
                    assert(element == e); // make sure we found the right element
                    lookups++;
                }
            }
            operations++;
        }
        end = now();
        seconds subtest = diff(start, end);
        printf("Computed %d operations (%d insertions %d deletions %d lookups) during %f\n", nexttests, insertions, deletions, lookups, subtest);
        insertions = 0;
        deletions = 0;
        lookups = 0;
    }
    // test trim
    s_array a = sda_create_empty();
    sda_init(&a, 1);
    for (size_t i = 0; i < ht_size(&ht); i++)
        if (ht.entries[i] != UNUSED)
            sda_insert(&a, ht.entries[i]);
    
    size_t count1 = ht_count(&ht);
    ht_trim(&ht);
    //print(&ht);
    size_t count2 = ht_count(&ht);
    assert(count1 == count2);
    size_t acount = sda_count(&a);
    size_t h;
    entry* en;
    for (size_t i = 0; i < acount; i++) {
        en = sda_remove_last(&a);
        h = hashfn(en, &ht);
        assert(ht.entries[h] != UNUSED);
    }


    prgEnd = now();
    seconds programTime = diff(prgStart, prgEnd);
    printf("Test completed, computed %d operations during %fs\n", operations, programTime);
    ht_free(&ht);
    return operations;
}

void test_sequence() {
    entry* del;
    hashtable ht = ht_create_empty();
    ht_init(&ht, 3, &hashfn, &compare);
    entry* e1 = createEntry('c', 1);
    entry* e2 = createEntry('/', 3);
    entry* e3 = createEntry('#', 7);
    entry* e4 = createEntry('<', 5);

    assert(ht_delete(&ht, e1) == UNUSED);
    assert(ht_lookup(&ht, e1) == UNUSED);

    ht_insert(&ht, e1);
    ht_insert(&ht, e2);
    ht_insert(&ht, e3);
    print(&ht);
    del = ht_delete(&ht, e2);
    assert(ht_size(&ht) == 3);
    assert(del != NULL);
    //free(del);
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

    printf("Test passed\n");
}