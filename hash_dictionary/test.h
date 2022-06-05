#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include "hashtable.h"
#include "../utils/timer.h"

typedef int value;
typedef char key;
typedef struct {
    value v;
    key k;
}entry;

#endif