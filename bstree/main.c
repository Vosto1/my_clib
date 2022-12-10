#include "test.h"

int main(void)
{
    test_sequence();
    auto_tests(1000, 1000);
    return 0;
}