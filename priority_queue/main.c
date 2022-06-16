#include "test_priority_queue.h"

int main(void)
{
    if (integrity_check(300))
        return 0;
    else
        return -1;
}