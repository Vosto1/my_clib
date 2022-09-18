#ifndef BSTREE_H
#define BSTREE_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../s_dynamic_array/sdarray.h"
#include "../utils/error.h"
#include "../datatype.h"

typedef struct treenode *bstree;

typedef struct datacont datacontainer;

struct datacont
{ // handle mutiple occurances of the same value in the tree
	cvoidp_t element;
	datacontainer *next;
};

struct treenode
{
	datacontainer cont;
	bstree parent;
	bstree left;
	bstree right;
};

bstree bst_create_empty();

void bst_insert(bstree *tree, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t));
voidp_t bst_remove(bstree *tree, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t));
bool bst_is_empty(const bstree tree);

bstree bst_merge(bstree *bst1, bstree *bst2, int (*compare)(cvoidp_t, cvoidp_t));

dim_t bst_toarray_preorder(const bstree tree, voidp_t **array);
dim_t bst_toarray_inorder(const bstree tree, voidp_t **array);
dim_t bst_toarray_postorder(const bstree tree, voidp_t **array);

cvoidp_t bst_find(const bstree tree, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t));

int bst_count(const bstree tree);
int bst_node_count(const bstree tree);

int bst_depth(const bstree tree);

int bst_mindepth(const bstree tree);

void bst_balance(bstree *tree, int (*compare)(cvoidp_t, cvoidp_t));

void bst_free(bstree *tree);
void bst_destroy(bstree *tree);

#endif