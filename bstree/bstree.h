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
typedef struct treenode* bstree;

typedef struct datacont datacontainer;

struct datacont { // handle mutiple occurances of the same value in the tree
	cvoidp element;
	datacontainer* next;
};

struct treenode {
	datacontainer cont;
	bstree parent;
	bstree left;
	bstree right;
};

bstree bst_create_empty();

void bst_insert(bstree* tree, voidp element, int (*compare)(cvoidp,cvoidp));
voidp bst_remove(bstree* tree, voidp element, int (*compare)(cvoidp,cvoidp));
bool bst_is_empty(const bstree tree);

void bst_merge(bstree* bst1, bstree* bst2, int (*compare)(cvoidp,cvoidp));

size_t bst_toarray_preorder(const bstree tree, voidp** array);
size_t bst_toarray_inorder(const bstree tree, voidp** array);
size_t bst_toarray_postorder(const bstree tree, voidp** array);

cvoidp bst_find(const bstree tree, voidp element, int (*compare)(cvoidp,cvoidp));

int bst_count(const bstree tree);
int bst_node_count(const bstree tree);

int bst_depth(const bstree tree);

int bst_mindepth(const bstree tree);

void bst_balance(bstree* tree, int (*compare)(cvoidp,cvoidp));

void bst_free(bstree* tree);
void bst_destroy(bstree* tree);

#endif