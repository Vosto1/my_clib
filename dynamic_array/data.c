#include "data.h"

int compareItems(void* i1, void* i2) {
    Item* item1 = (Item*)i1;
    Item* item2 = (Item*)i2;
    return item1->value - item2->value;
}