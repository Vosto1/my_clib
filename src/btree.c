#include "btree.h"

btree bt_new_node(void* value)
{
    btree n = (btree)malloc(sizeof(struct treeNode));
    if (n != NULL)
    {
        n->value = value;
        n->left = NULL;
        n->right = NULL;
        return n;
    }
    else
    {
        return NULL;
    }
}

bool bt_left_is_null(const btree bst)
{
    return bst->left == NULL;
}

bool bt_right_is_null(const btree bst)
{
    return bst->right == NULL;
}

bool bt_is_empty(const btree tree)
{
    return tree == NULL;
}

btree bt_create_empty(void)
{
    return NULL;
}

int bt_count(const btree tree)
{
    if (tree == NULL)
        return 0;
    return 1 + bt_count(tree->right) + bt_count(tree->left);
}

static void btfree(btree *tree, void (*freeObject)(void*))
{
    if ((*tree) != NULL)
    {
        btfree(&(*tree)->right, freeObject);
        btfree(&(*tree)->left, freeObject);
        // free element in the node
        (*freeObject)((*tree)->value);
        // free the node
        free((*tree));
        (*tree) = NULL;
    }
}

bool bt_free(btree *tree, void (*freeObject)(void*))
{
    if ((*tree) != NULL)
    {
        btfree(tree, freeObject);
        (*tree) = NULL;
        return true;
    }
    return false;
}

static void btdestr(btree *tree)
{
    if ((*tree) != NULL)
    {
        btdestr(&(*tree)->right);
        btdestr(&(*tree)->left);
        // free node
        free((*tree));
        (*tree) = NULL;
    }
}

// free only nodes (not elements in the tree)
bool bt_destroy(btree *tree)
{
    if ((*tree) != NULL)
    {
        btdestr(tree);
        return true;
    }
    return false;
}