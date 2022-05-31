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
void heapPrintTree(heap* h) {
    int y = 0;
    int x = 0;
    for (int i = 0; i < s_heapSize(h); i++) {
        for (int j = 0; j < pow(2, i) && j + pow(2, i) <= s_heapSize(h); j++) {
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