#include "test_priority_queue.h"

#define PASSED 0
#define NOT_PASSED -1

int main(void)
{
    if (integrity_check(100))
        return PASSED;
    else
        return NOT_PASSED;
}