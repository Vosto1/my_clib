#include "btree.h"

btree bt_new_node(voidp_t value)
{
    btree n = (btree)malloc(sizeof(struct treenode));
    if (n != NULL)
    {
        n->value = value;
        n->left = NULL;
        n->right = NULL;
        return n;
    }
    else
    {
        errcset(EBTREE_NODE_MEMALLOC);
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

static void btfree(btree *tree)
{
    if ((*tree) != NULL)
    {
        btfree(&(*tree)->right);
        btfree(&(*tree)->left);
        // free element in the node
        free((*tree)->value);
        // free the node
        free((*tree));
        (*tree) = NULL;
    }
}

void bt_free(btree *tree)
{
    if ((*tree) != NULL)
    {
        btfree(tree);
    }
    else
    {
        errcset(EBTREE_FREENULLPTR);
    }
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
void bt_destroy(btree *tree)
{
    if ((*tree) != NULL)
    {
        btdestr(tree);
    }
    else
    {
        errcset(EBTREE_FREENULLPTR);
    }
}