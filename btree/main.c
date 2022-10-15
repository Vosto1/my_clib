#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "btree.h"

typedef char *value;

static value createValue(int v)
{
    value a = (value)malloc(sizeof(char));
    return a;
}

static void freeobj(voidp_t i)
{
    free(i);
}

static int rascii()
{
    return rand() % 94 + 32; // ascii character value span
}

int main(void)
{
    srand(time(NULL));
    btree bt = bt_create_empty();

    bt = bt_new_node(createValue(rascii()));
    bt->left = bt_new_node(createValue(rascii()));
    bt->right = bt_new_node(createValue(rascii()));

    assert(bt_count(bt) == 3);
    assert(!bt_left_is_null(bt));
    assert(!bt_right_is_null(bt));
    assert(!bt_is_empty(bt));

    value arr[3];
    arr[0] = bt->value;
    arr[1] = bt->left->value;
    arr[2] = bt->right->value;

    assert(bt_destroy(&bt));

    assert(bt == NULL);

    bt = bt_create_empty();

    bt = bt_new_node(arr[0]);
    bt->left = bt_new_node(arr[1]);
    bt->right = bt_new_node(arr[2]);

    assert(bt_free(&bt, freeobj));

    printf("Tests passed.\n");

    return 0;
}