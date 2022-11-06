#include "test.h"

static void freeobj(void* a)
{
    char* ch = (char*)a;
    free(ch);
}

static char* new_char(int code)
{
    char* ch = (char*)malloc(sizeof(char));
    *ch = code;
    return ch;
}

void test()
{
    
}