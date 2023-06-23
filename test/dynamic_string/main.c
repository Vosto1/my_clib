#include <time.h>
#include "test.h"

int main()
{
    // remove seeding before debugging!
    srand(time(0));
    //test();
    auto_tests(100);
    return 0;
}