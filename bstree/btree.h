#ifndef BSTREE_H
#define BSTREE_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/error.h"

typedef void* voidp;
typedef const void* cvoidp;
typedef struct treenode node;

struct treenode {
	cvoidp element;
	node* parent;
	node* left;
	node* right;
};

typedef node* btree;

btree bt_create_empty();

void bt_insert(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp));
cvoidp bt_remove(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp));
bool bt_is_empty(const btree tree);

void bt_merge(btree* bt1, btree* bt2, int (*compare)(cvoidp,cvoidp));

cvoidp* bt_toarray_preorder(const btree tree);
cvoidp* bt_toarray_inorder(const btree tree);
cvoidp* bt_toarray_postorder(const btree tree);

cvoidp bt_find(const btree tree, voidp element, int (*compare)(cvoidp,cvoidp));
cvoidp bt_max(const btree* tree, int (*compare)(cvoidp,cvoidp));
cvoidp bt_min(const btree* tree, int (*compare)(cvoidp,cvoidp));



int bt_count(const btree tree);

int bt_depth(const btree tree);

int bt_mindepth(const btree tree);

void bt_balance(btree* tree, int (*compare)(cvoidp,cvoidp));

void bt_free(btree* tree);

#endif