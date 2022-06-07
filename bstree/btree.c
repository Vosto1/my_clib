#include "btree.h"

static node* new_node(cvoidp element) {
    node* n = (node*)malloc(sizeof(node));
    if (n != NULL) {
        n->element = element;
        n->left = NULL;
        n->right = NULL;
        n->parent = NULL;
        return n;
    } else { 
        errcset(EBTREE_NODE_MEMALLOC);
        return NULL;
    }
}

static bool is_bigger_or_equal(cvoidp element1, cvoidp element2, int (*compare)(cvoidp,cvoidp)) {
    return (*compare)(element1, element2) >= 0;
}

static bool is_smaller(cvoidp element1, cvoidp element2, int (*compare)(cvoidp,cvoidp)) {
    return (*compare)(element1, element2) < 0;
}

static bool is_bigger(cvoidp element1, cvoidp element2, int (*compare)(cvoidp,cvoidp)) {
    return (*compare)(element1, element2) > 0;
}

static bool is_equal(cvoidp element1, cvoidp element2, int (*compare)(cvoidp,cvoidp)) {
    return (*compare)(element1, element2) == 0;
}

static bool left_isnull(const btree bt) {
    return bt->left == NULL;
}

static bool right_isnull(const btree bt) {
    return bt->right == NULL;
}

btree bt_create_empty(void) {
    return NULL;
}

static void insert(btree* current, btree* prev, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (current == NULL) {
        *current = new_node(element);
        (*current)->parent = *prev;
        // set parents' pointer to point to new node
        if (is_bigger_or_equal((*current)->element, (*prev)->element, compare)) {
            (*prev)->right = *current;
        } else {
            (*prev)->left = *current;
        }
    } 
    else if (is_bigger_or_equal((*current)->element, element, compare)) // if tree->e is bigger or equal to e
        insert(current, &(*current)->right, element, compare);
    else // if tree->e is smaller than e
        insert(current, &(*current)->left, element, compare);
}

void bt_insert(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (*tree == NULL) {
        *tree = new_node(element);
    }
    else if (is_bigger_or_equal((*tree)->element, element, compare)) // if tree->e is bigger or equal to e
        insert(tree, &(*tree)->right, element, compare);
    else // if tree->e is smaller than e
        insert(tree, &(*tree)->left, element, compare);
}

static btree* findSmallestNodeRight(btree* bt, int (*compare)(cvoidp,cvoidp)) {
    btree* tmp = &(*bt)->right;
    while (!left_isnull(*tmp)) {
        tmp = &(*tmp)->left;
    }
    return tmp;
}

static btree* findLargestNodeLeft(btree* bt, int (*compare)(cvoidp,cvoidp)) {
    btree* tmp = &(*bt)->left;
    while (!right_isnull(*tmp)) {
        tmp = &(*tmp)->right;
    }
    return tmp;
}

static cvoidp rmWithRightChild(btree* bt) {
    if ((*bt)->parent->right == (*bt)) {
        (*bt)->parent->right = (*bt)->right;
    } else {
        (*bt)->parent->left = (*bt)->right;
    }
    cvoidp element = (*bt)->element;
    (*bt)->right = NULL;
    free((*bt));
    return element;
}

static cvoidp rmWithLeftChild(btree* bt) {
    if ((*bt)->parent->right == (*bt)) {
        (*bt)->parent->right = (*bt)->left;
    } else {
        (*bt)->parent->left = (*bt)->left;
    }
    cvoidp element = (*bt)->element;
    (*bt)->left = NULL;
    free((*bt));
    return element;
}

static cvoidp rmWithTwoChildren(btree* bt, int (*compare)(cvoidp,cvoidp)) {
    // get replacement node
    btree* tmp;
    int random = rand() % 2;
    if (random == 1) {
        tmp = findSmallestNodeRight(bt, compare);
        if ((*tmp)->right != NULL) {
            (*tmp)->parent->left = (*tmp)->right;
            (*tmp)->right = NULL;
        }
    } else {
        tmp = findLargestNodeLeft(bt, compare);
        if ((*tmp)->left != NULL) {
            (*tmp)->parent->right = (*tmp)->left;
            (*tmp)->left = NULL;
        }
    }
    
    // replace node to remove
    if ((*bt)->parent->right == (*bt)) {
        (*bt)->parent->right = (*tmp);
        (*tmp)->left = (*bt)->left;
        (*tmp)->right = (*bt)->right;
    } else {
        (*bt)->parent->left = (*tmp);
        (*tmp)->left = (*bt)->left;
        (*tmp)->right = (*bt)->right;
    }

    // finalize
    cvoidp element = (*bt)->element;
    (*bt)->element = NULL;
    free((*bt));
    return element;
}

static cvoidp rmWithNoChildren(btree* bt) {
    if ((*bt)->parent->right == (*bt)) {
        (*bt)->parent->right = NULL;
    } else {
        (*bt)->parent->left = NULL;
    }
    cvoidp element = (*bt)->element;
    (*bt)->element = NULL;
    free((*bt));
    return element;
}

cvoidp bt_remove(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (tree != NULL) {
        if (is_equal((*tree)->element, element, compare)) {
            if (left_isnull(*tree) && right_isnull(*tree)) { // if the node doesnt have children
                return rmWithNoChildren(tree);
            } else if (left_isnull(*tree)) { // if the node has a right child
                return rmWithRightChild(tree);
            } else if (right_isnull(*tree)) { // if the node has a left child
                return rmWithLeftChild(tree);
            } else { // if the node has two children
                return rmWithTwoChildren(tree, compare);
            }
        } else { // tree->element was not equal to element
            if (is_bigger_or_equal((*tree)->element, element, compare)) {
                return bt_remove(&(*tree)->right, element, compare);
            } else {
                return bt_remove(&(*tree)->left, element, compare);
            }
        }
    } else { // btree doesnt contain element
        return NULL;
    }
}

bool bt_is_empty(const btree tree) {
    return tree == NULL;
}

static cvoidp* writeSortedToArray(const btree tree) {
    return bt_toarray_inorder(tree);
}

static void insertFromSortedArray(btree* tree, cvoidp* a, int start, int end, int (*compare)(cvoidp,cvoidp)) {
    if (start > end)
        return;

    int mid = (start + end) / 2;
    bt_insert(tree, (voidp)a[mid], compare);

    insertFromSortedArray(tree, a, start, mid - 1, compare);
    insertFromSortedArray(tree, a, mid + 1, end, compare);
}

// free only nodes (not elements in the tree)
static void free_tree(btree* tree) {
    if ((*tree) != NULL) {
        bt_free(&(*tree)->right);
        bt_free(&(*tree)->left);
        free((*tree));
        (*tree) = NULL;
    } else {
        errcset(EBTREE_FREENULLPTR);
    }
}

void bt_merge(btree* bt1, btree* bt2, int (*compare)(cvoidp,cvoidp)) {
    cvoidp* a = writeSortedToArray(*bt2);
    int size = bt_count(*bt2);
    insertFromSortedArray(bt1, a, 0, size - 1, compare);
    free(a);
    // free only nodes of bt2
    free_tree(bt2);
}

static void preorder(const btree tree, cvoidp* a, int* index) {
    if (tree != NULL) {
        a[*index] = tree->element;
        *index += 1;
        if (tree->left != NULL)
            preorder(tree->left, a, index);
        if (tree->right != NULL)
            preorder(tree->right, a, index);
    }
}

static void inorder(const btree tree, cvoidp* a, int* index) {
    if (tree != NULL) {
        if (tree->left != NULL)
            inorder(tree->left, a, index);
        a[*index] = tree->element;
        *index += 1;
        if (tree->right != NULL)
            inorder(tree->right, a, index);
    }
}

static void postorder(const btree tree, cvoidp* a, int* index) {
    if (tree != NULL) {
        if (tree->left != NULL)
            postorder(tree->left, a, index);
        if (tree->right != NULL)
            postorder(tree->right, a, index);
        a[*index] = tree->element;
        *index += 1;
    }
}

static cvoidp* get_array(const btree tree, void (*order)(const btree,cvoidp*,int*)) {
    if (tree != NULL) {
        cvoidp* a = (cvoidp*)malloc(sizeof(cvoidp) * bt_count(tree));
        if (a != NULL) {
            int index = 0;
            (*order)(tree, a, &index);
            return a;
        } else {
            errcset(EBTREE_WRITEARR_MEMALLOC);
            return NULL;
        }
    } else {
        errcset(EBTREE_WRITEARR_EMPTY);
        return NULL;
    }
}

cvoidp* bt_toarray_preorder(const btree tree) {
    return get_array(tree, &preorder);
}

cvoidp* bt_toarray_inorder(const btree tree) {
    return get_array(tree, &inorder);
}

cvoidp* bt_toarray_postorder(const btree tree) {
    return get_array(tree, &postorder);
}

cvoidp bt_find(const btree tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (tree != NULL) {
        if (is_equal(tree->element, element, compare)) {
            return tree->element;
        } else if (is_bigger(tree->element, element, compare)) {
            return bt_find(tree->right, element, compare);
        } else {
            return bt_find(tree->left, element, compare);
        }
    } else { // btree doesnt contain element
        return NULL;
    }
}

cvoidp bt_max(const btree* tree, int (*compare)(cvoidp,cvoidp)) {
    btree* tmp = (btree*)tree;
    while ((*tmp)->right != NULL) {
        tmp = &(*tmp)->right;
    }
    return (*tmp)->element;
}

cvoidp bt_min(const btree* tree, int (*compare)(cvoidp,cvoidp)) {
    btree* tmp = (btree*)tree;
        while ((*tmp)->left != NULL) {
            tmp = &(*tmp)->left;
        }
        return (*tmp)->element;
}

int bt_count(const btree tree) {
    if (tree != NULL) {
        return 1 + bt_count(tree->right) + bt_count(tree->left);
    }
    return 0;
}

int bt_depth(const btree tree) {
    int x = 1, y = 1;
	if (tree == NULL)
		return 0;
	x += bt_depth(tree->left);
	y += bt_depth(tree->right);
	if (x >= y)
		return x;
	else
		return y;
}

int bt_mindepth(const btree tree) {
    if (tree != NULL)
        return (int)ceil(log((double)bt_count(tree) + 1.0f) / log(2.0f));
    return 0;
}

static btree buildTreeFromArray(cvoidp* a, int start, int end) {
    if (start > end)
        return NULL;
    
    int mid = (start + end) / 2;
    btree bt = new_node(a[mid]);

    bt->right = buildTreeFromArray(a, mid + 1, end);
    bt->left = buildTreeFromArray(a, start, mid - 1);

    return bt;
}

void bt_balance(btree* tree, int (*compare)(cvoidp,cvoidp)) {
    if ((*tree) != NULL) {
        cvoidp* a = writeSortedToArray((*tree));
        if (a != NULL) {
            int treesize = bt_count(*tree);
            btree new = buildTreeFromArray(a, 0, treesize - 1);
            if (bt_count(new) == bt_count(*tree) && bt_depth(new) == bt_depth(*tree)) {
                free(a);
                free_tree(tree);
                *tree = new;
            } else {
                // rollback (free)
                free(a);
                // rollback (free)
                free_tree(&new);
                errcset(EBTREE_BALANCE);
            }
        } else {
            errcset(EBTREE_WRITEARR_MEMALLOC);
        }
    }
}

void bt_free(btree* tree) {
    if ((*tree) != NULL) {
        bt_free(&(*tree)->right);
        bt_free(&(*tree)->left);
        free((voidp)(*tree)->element);
        free((*tree));
        (*tree) = NULL;
    } else {
        errcset(EBTREE_FREENULLPTR);
    }
}

