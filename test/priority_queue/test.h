#ifndef TEST_PRIORITY_QUEUE_H
#define TEST_PRIORITY_QUEUE_H

#define __TESTING__
#include "priorityqueue.h"
#include "timer.h"
#include <assert.h>
#include <stdbool.h>

// test function
bool integrity_check(int n);

// user defined functions needed for the sheap
int compare(const void* x, const void* y);
void print(size_t used, void* a[]);

#endif