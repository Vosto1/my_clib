#ifndef BSTREE_H
#define BSTREE_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../s_dynamic_array/s_dynamic_array.h"
#include "../utils/error.h"

typedef void* voidp;
typedef const void* cvoidp;
typedef struct treenode* btree;

typedef struct datacont datacontainer;

struct datacont { // handle mutiple occurances of the same value in the tree
	cvoidp element;
	datacontainer* next;
};

struct treenode {
	datacontainer cont;
	btree parent;
	btree left;
	btree right;
};

btree bt_create_empty();

void bt_insert(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp));
voidp bt_remove(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp));
bool bt_is_empty(const btree tree);

void bt_merge(btree* bt1, btree* bt2, int (*compare)(cvoidp,cvoidp));

size_t bt_toarray_preorder(const btree tree, voidp** array);
size_t bt_toarray_inorder(const btree tree, voidp** array);
size_t bt_toarray_postorder(const btree tree, voidp** array);

cvoidp bt_find(const btree tree, voidp element, int (*compare)(cvoidp,cvoidp));
cvoidp bt_rightmost(const btree* tree, int (*compare)(cvoidp,cvoidp));
cvoidp bt_leftmost(const btree* tree, int (*compare)(cvoidp,cvoidp));

int bt_count(const btree tree);
int bt_node_count(const btree tree);

int bt_depth(const btree tree);

int bt_mindepth(const btree tree);

void bt_balance(btree* tree, int (*compare)(cvoidp,cvoidp));

void bt_free(btree* tree);
void bt_destroy(btree* tree);

#endif