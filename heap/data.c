#include "data.h"


// dynamic array
int compare(void* x, void* y) {
    Item* item0 = (Item*)x;
    Item* item1 = (Item*)y;
    if(item0->key > item1->key) return 1;
    else if (item0->key < item1->key) return -1;
    else return 0;
}

// heap
void decrementKey(Item* base, void** out) {
    void* o = *out;
    Item* oi = (Item*)o;
    base->key -= 1.0f; // decrement the smallest key in the heap
    oi->key = base->key; // set out key to new key
    base->key += 1.0f; // rollback, because we are decrementing an actual item in the heap
}

// guarantees the smallest key unless the heap has been changed after
void minKey(Data base, Data* out) {
    Item* min = (Item*)base;
    *out = (Item*)malloc(sizeof(Item));
    decrementKey(min, out);
}

void* setKey(void* item, void* key) {
    Item* i = (Item*)item;
    Item* k = (Item*)key;
    i->key = k->key;
    return i;
}

void heapPrintTree(heap* h) {
    int y = 0;
    int x = 0;
    for (int i = 0; i < heapSize(h); i++) {
        for (int j = 0; j < pow(2, i) && j + pow(2, i) <= heapSize(h); j++) {
            x = j + (int)pow(2, i) - 1;
            y = h->items.used;
            if (y > x) {
                Item* item = (Item*)h->items.array[x];
                printf("[k%f|%c]", item->key, item->element);
            }
            else printf("----");
        }
        printf("\n");
    }
}