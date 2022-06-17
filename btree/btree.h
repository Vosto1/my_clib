/**
 * @file btree.h
 * @author Isac Hutchings (isac.hutchings@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef BTREE_H
#define BTREE_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../s_dynamic_array/s_dynamic_array.h"
#include "../utils/error.h"

typedef void *voidp_t;
typedef const void *cvoidp_t;
typedef struct treenode *btree;

struct treenode
{
	voidp_t value;
	btree left;
	btree right;
};

btree bt_create_empty();
btree bt_new_node(voidp_t value);

bool bt_left_is_null(const btree bst);
bool bt_right_is_null(const btree bst);
bool bt_is_empty(const btree tree);

int bt_count(const btree tree);

void bt_free(btree *tree);
void bt_destroy(btree *tree);

#endif