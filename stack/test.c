#include "test.h"

void freeobj(void* o)
{
    free(o);
}

char rascii()
{
    return (char)rand() % 94 + 32;
}

char* new_item()
{
    char* i = (char*)malloc(sizeof(char));
    if (i == NULL)
    {
        printf("malloc item failed\n");
        exit(EXIT_FAILURE);
    }
    *i = rascii();
    return i;
}

void push(stack*s)
{
    int c = st_count(s);
    assert(st_push(s, new_item()));
    assert(c + 1 == st_count(s));
}

void pop(stack*s)
{
    int c = st_count(s);
    if (!st_is_empty(s))
    {
        char* i = st_pop(s);
        assert(i != NULL);
        free(i);
        assert(c - 1 == st_count(s));
        return;
    }
}

void peek(stack*s)
{
    int c = st_count(s);
    if (!st_is_empty(s))
    {
        const char* i = st_peek(s);
        assert(i != NULL);
        assert(c == st_count(s));
        return;
    }
}

void test(int amount)
{
    int size = 10;
    stack s = st_create_empty();
    assert(st_init(&s, size, &freeobj) == size);
    srand(time(0));
    printf("running tests (%d)\n", amount);

    for (int i = 0; i < amount; i++)
    {
        switch (rand() % 3)
        {
            case 0: // fall through
            case 1: // fall through
            case 2: push(&s); break;
            case 3: pop(&s); break;
            case 4: peek(&s); break;
        default:
            // cant happen
            printf("wut?");
            break;
        }
    }

    st_free(&s);
    printf("success\n");
}