#ifndef BSTREE_H
#define BSTREE_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../dynamic_array/darray.h"
//#include "../utils/error.h"
#include "../datatype.h"

typedef struct treeNode *bstree;

typedef struct datacont dataContainer;

struct datacont
{ // handle mutiple occurances of the same value in the tree
	const void* element;
	dataContainer *next;
};

struct treeNode
{
	dataContainer cont;
	bstree parent;
	bstree left;
	bstree right;
};

bstree bst_create_empty();

void bst_insert(bstree *tree, void* element, int (*compare)(const void*, const void*));
void* bst_remove(bstree *tree, void* element, int (*compare)(const void*, const void*));
bool bst_is_empty(const bstree tree);

bstree bst_merge(bstree *bst1, bstree *bst2, int (*compare)(const void*, const void*));

uint bst_toarray_preorder(const bstree tree, void* **array);
uint bst_toarray_inorder(const bstree tree, void* **array);
uint bst_toarray_postorder(const bstree tree, void* **array);

const void* bst_find(const bstree tree, void* element, int (*compare)(const void*, const void*));

int bst_count(const bstree tree);
int bst_node_count(const bstree tree);

int bst_depth(const bstree tree);

int bst_mindepth(const bstree tree);

bool bst_balance(bstree *tree, int (*compare)(const void*, const void*));

bool bst_free(bstree *tree, void (*freeObject)(void*));
bool bst_destroy(bstree *tree);

#endif