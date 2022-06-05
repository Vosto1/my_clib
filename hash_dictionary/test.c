#include "test.h"

int compare(cvoidp e1, cvoidp e2) {
    entry* en1 = (entry*)e1;
    entry* en2 = (entry*)e2;
    return (int)en1->k - (int)en2->k;
}

size_t hash(cvoidp e, const hashtable* ht) {
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
    size_t i = 0;
    for (; i < ht_size(ht); i++)
        if (ht->entries[i] != UNUSED)
            break;
    return (entry*)ht->entries[i];
}

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


void auto_tests(int tests, int mod) {
    srand(time(NULL));

    hashtable ht = ht_createEmpty();
    ht_init(&ht, 10, &hash, &compare);
    ticks start, end, prgStart, prgEnd;
    unsigned int operations, deletions, insertions, lookups;
    unsigned int random, val;
    entry* element;
    entry* e; 

    prgStart = now();
    for (unsigned int i = 0; i < tests; i++) {
        random = rand() % 100;
        val = rand() % 94 + 32; // ascii character value span
        start = now();
        if (random < 70) { // insert
            element = createEntry((char)val, val);
            assert(ht_insert(&ht, element) != -1);
        }
        else if (random > 70 && random < 90) { // delete
            ht_delete(&ht, )

        }
        else { // look up

        }
        end = now();
    }
    prgEnd = now();
    seconds programTime = diff(prgStart, prgEnd);
}

void test_sequence() {
    entry* del;
    hashtable ht = ht_createEmpty();
    ht_init(&ht, 3, &hash, &compare);
    entry* e1 = createEntry('c', 1);
    entry* e2 = createEntry('/', 3);
    entry* e3 = createEntry('#', 7);
    entry* e4 = createEntry('<', 5);
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