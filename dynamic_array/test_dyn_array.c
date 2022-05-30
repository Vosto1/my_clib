#define _CRT_SECURE_NO_WARNINGS
#include "dynamic_array.h"
#include "../utils/error.h"
#include "assert.h"
#include "../utils/timer.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

// datatype -->
#include "data.h"
// datatype <--

typedef struct {
    int used;
    int size;
    double ratio;
}stats;

typedef struct {
    seconds s;
    int operation_amount;
    char operation[64];
}test_result;

int _max(int x, int y) {
    return x > y ? x : y;
}

double ratio(int used, int size) {
    return (double)used/(double)size;
}

void print_results(test_result res) {
    printf("computed %d %s operations during %f seconds.\n", res.operation_amount, res.operation, res.s);
    // write stats to file (in some good format, for desmos or wolfram for example)
}

void print_status(stats stat) {
    system("clear");
    printf("-------------------------\n");
    printf("-------array status------\n");
    printf("array size: %d\n", stat.size);
    printf("used indecies: %d\n", stat.used);
    printf("used vs size ratio: %f\n", stat.ratio);
    printf("-------------------------\n\n");
}

Item* createItem(int value) {
    Item* item = (Item*)malloc(sizeof(Item));
    item->value = value;
    return item;
}

void printData(array* a) {
    Item* item;
    for (int i = 0; i < a->used; i++) {
        item = (Item*)a->array[i];
        printf("value at %d: %d\n", i, item->value);
    }
    printf("used: %d\nsize: %d\n\n", a->used, a->size);
}

void insert_n(array * a, int n) {
    size_t e;
    Item* item;
    for (int j = 0; j < n; j++) {
        item = createItem(rand() % 1000);
        e = a->used + 1;
        assert(arrayInsert(a, (void*)item) == e);
        errorHandler();
    }
    assert(a->used == n);
}

void remove_all(array* a) {
    Item* d;
    for (int j = 0; a->used != 0; j++) {
        d = (Item*)arrayRemoveLast(a);
        assert(d != NULL);
        free(d);
        errorHandler();
    }
    assert(a->used == 0);
}

void auto_tests(int n, int mod) {
    array a = createEmptyDynamicArray();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(initDynamicArray(&a, 10, &compareItems) == 10);
    errorHandler();
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    Item item;
    Item* d;
    srand(time(0));
    stats stat;
    test_result test_r;
    ticks start;
    ticks end;
    char operation[64];

    for (int i = 0; i < n; i++) {
        int next_tests = rand() % mod;
        int type = rand() % 5;
        
        switch (type) {
            case 0:
                sprintf(operation, "insert item");
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    d = createItem(rand() % 1000);
                    assert(arrayInsert(&a, (void*)d) != -1);
                    errorHandler();
                }
                end = now();
                remove_all(&a);
            break;
            case 1:
                sprintf(operation, "remove item");
                insert_n(&a, next_tests);
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    d = (Item*)arrayRemoveItem(&a, a.array[rand() % a.used]);
                    assert(d != NULL);
                    free(d);
                    errorHandler();
                }
                end = now();
            break;
            case 2:
                sprintf(operation, "remove at");
                insert_n(&a, next_tests);
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    d = (Item*)arrayRemoveAt(&a, rand() % a.used);
                    assert(d != NULL);
                    free(d);
                    errorHandler();
                }
                end = now();
            break;
            case 3:
                sprintf(operation, "remove last");
                insert_n(&a, next_tests);
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    d = (Item*)arrayRemoveLast(&a);
                    assert(d != NULL);
                    free(d);
                    errorHandler();
                }
                end = now();
            break;
            case 4:
                sprintf(operation, "convert + union");
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    int size = rand() % 100;
                    Item** b = (Item**)malloc(sizeof(Item*)*size);
                    if (!b) {
                        printf("malloc error in test function.\n");
                        exit(-100);
                    }
                    for (int k = 0; k < size; k++)
                        b[k] = createItem(rand() % 1000);
                    dynamicArray c = createEmptyDynamicArray();
                    assert(initDynamicArray(&c, size, &compareItems) == size);
                    errorHandler();
                    assert(convert(&c, (void*)b, size, &compareItems) == size);
                    errorHandler();
                    assert(arrayUnion(&a, &c) != -1);
                    errorHandler();
                    free(b);
                }
                end = now();
                remove_all(&a);
            break;
        }
        end = now();
        seconds s = diff(start, end);
        strcpy(test_r.operation, operation);
        test_r.operation_amount = next_tests;
        test_r.s = s;
        print_results(test_r);
    }
    freeArray(&a);
    printf("Test passed.\n");
}

void test_sequence() {
    Item item;
    Item* itemptr;

    array a = createEmptyDynamicArray();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(initDynamicArray(&a, 10, &compareItems) != -1);
    errorHandler();
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    for (int i = 0; i < 10; i++) {
        itemptr = createItem(i);
        assert(arrayInsert(&a, (void*)itemptr) != -1);
        errorHandler();
        assert(a.used == (i + 1));
        itemptr = (Item*)a.array[i];
        assert(itemptr->value == i);
    }

    printData(&a);

    for (int i = 0; i < 5; i++) {
        itemptr = createItem(i + a.used);
        assert(arrayInsert(&a, (void*)itemptr) != -1);
        errorHandler();
        itemptr = (Item*)a.array[a.used - 1];
        assert(itemptr->value == i + (a.used - 1));
    }
    assert(a.size == 20);
    assert(a.used == 15);

    printData(&a);
    
    itemptr = createItem(1);
    assert(arrayInsert(&a, (void*)itemptr) != -1);

    itemptr = createItem(2);
    assert(arrayInsert(&a, (void*)itemptr) != -1);

    itemptr = createItem(3);
    assert(arrayInsert(&a, (void*)itemptr) != -1);

    itemptr = createItem(4);
    assert(arrayInsert(&a, (void*)itemptr) != -1);

    printData(&a);
    printf("%d %d\n\n", a.used, a.size);

    itemptr = createItem(5);
    assert(arrayInsert(&a, (void*)itemptr) != -1);
    for (int i = a.used; i > 5; i--) {
        itemptr = (Item*)arrayRemoveLast(&a);
        assert(itemptr != NULL);
        free(itemptr);
        assert(a.used == (i - 1));
    }
    assert(a.size == 10);

    printData(&a);

    item.value = 4;
    itemptr = (Item*)arrayRemoveItem(&a, (void*)&item);
    assert(itemptr != NULL);
    assert(itemptr->value == item.value);
    free(itemptr);

    item.value = 2;
    itemptr = (Item*)arrayRemoveItem(&a, (void*)&item);
    assert(itemptr != NULL);
    assert(itemptr->value == item.value);
    free(itemptr);
    assert(a.used == 3);
    
    printData(&a);
    
    itemptr = (Item*)arrayRemoveAt(&a, 0);
    assert(itemptr != NULL);
    assert(itemptr->value == 0);
    free(itemptr);
    assert(a.size == 5);

    printData(&a);

    freeArray(&a);
    printf("Tests passed.\n");
}

// dyn array test
int main(void) {
    //test_sequence();
    auto_tests(100, 1000000);
    return SUCCESS;
}