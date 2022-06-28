#ifndef TEST_PRIORITY_QUEUE_H
#define TEST_PRIORITY_QUEUE_H

#define __TESTING__
#include "priority_queue.h"
#include "../utils/timer.h"
#include <assert.h>
#include <stdbool.h>

// test function
bool integrity_check(int n);

// user defined functions needed for the sheap
int compare(cvoidp_t x, cvoidp_t y);
void print(dim_t used, voidp_t a[]);

#endif