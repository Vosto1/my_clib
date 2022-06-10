#include "btree.h"

static void print(voidp e) {
    char* c = (char*)e;
    printf("%c",*c);
}

static btree new_node(cvoidp element) {
    btree n = (btree)malloc(sizeof(struct treenode));
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
    if (*current == NULL) {
        *current = new_node(element);
        (*current)->parent = *prev;
    } 
    else if (is_bigger_or_equal((*current)->element, element, compare)) // if tree->e is bigger or equal to e
        insert(&(*current)->right, current, element, compare);
    else // if tree->e is smaller than e
        insert(&(*current)->left, current, element, compare);
}

void bt_insert(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (*tree == NULL) {
        *tree = new_node(element);
    }
    else if (is_bigger_or_equal((*tree)->element, element, compare)) // if tree->e is bigger or equal to e
        insert(&(*tree)->right, tree, element, compare);
    else // if tree->e is smaller than e
        insert(&(*tree)->left, tree, element, compare);
}

static btree* findSmallestNodeRight(btree* bt, int (*compare)(cvoidp,cvoidp));
static btree* findLargestNodeLeft(btree* bt, int (*compare)(cvoidp,cvoidp));
static voidp rmWithNoChildren(btree* bt);
static voidp rmWithRightChild(btree* bt);
static voidp rmWithLeftChild(btree* bt);
static voidp rmWithTwoChildren(btree* bt, int (*compare)(cvoidp,cvoidp));

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

static voidp rmWithNoChildren(btree* bt) {
    btree rm = *bt;
    if (rm->parent != NULL)
        if (rm->parent->right == rm)
            rm->parent->right = NULL;
        else
            rm->parent->left = NULL;
    //*bt = NULL; // set pointer in the tree to NULL
    voidp e = (voidp)rm->element;
    free(rm);
    rm = NULL;
    return e;
}

static voidp rmWithRightChild(btree* bt) {
    btree rm = *bt;
    btree child = rm->right;
    btree parent = rm->parent;

    if (rm->parent != NULL)
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;

    child->parent = parent;
    voidp e = (voidp)rm->element;
    free(rm);
    return e;
}

static voidp rmWithLeftChild(btree* bt) {
    btree rm = *bt;
    btree child = rm->left;
    btree parent = rm->parent;

    if (rm->parent != NULL)
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;
    
    child->parent = parent;
    voidp e = (voidp)rm->element;
    free(rm);
    return e;
}

static voidp rmWithTwoChildren(btree* bt, int (*compare)(cvoidp,cvoidp)) {
    btree rm = *bt;
    btree* repl;
    int random = rand() % 2;
    if (random == 1) {
        repl = findSmallestNodeRight(bt, compare);
    } else {
        repl = findLargestNodeLeft(bt, compare);
    }

    voidp element = bt_remove(bt, (voidp)(*repl)->element, compare);
    btree parent = rm->parent;

    btree lchild = rm->left;
    btree rchild = rm->right;
    
    btree node = new_node(element);
    if (lchild != NULL)
        lchild->parent = node;
    if (rchild != NULL)
        rchild->parent = node;

    if (parent != NULL)
        if (parent->right == rm)
            parent->right = node;
        else
            parent->left = node;


    node->left = lchild;
    node->right = rchild;
    node->parent = rm->parent;
    
    (*bt) = node;
    voidp e = (voidp)rm->element;
    free(rm);
    return e;
}

voidp bt_remove(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (*tree != NULL) {
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

static size_t writeSortedToArray(const btree tree, voidp** array) {
    return bt_toarray_inorder(tree, array);
}

static void insertFromSortedArray(btree* tree, voidp* a, int start, int end, int (*compare)(cvoidp,cvoidp)) {
    if (start > end)
        return;

    int mid = (start + end) / 2;
    bt_insert(tree, a[mid], compare);

    insertFromSortedArray(tree, a, start, mid - 1, compare);
    insertFromSortedArray(tree, a, mid + 1, end, compare);
}

// free only nodes (not elements in the tree)
static void free_tree(btree* tree) {
    if ((*tree) != NULL) {
        free_tree(&(*tree)->right);
        free_tree(&(*tree)->left);
        free((*tree));
        (*tree) = NULL;
    } else {
        errcset(EBTREE_FREENULLPTR);
    }
}

void bt_merge(btree* bt1, btree* bt2, int (*compare)(cvoidp,cvoidp)) {
    if ((*bt2) == NULL)
        return; 
    voidp* array;
    size_t size = writeSortedToArray(*bt2, &array);
    insertFromSortedArray(bt1, array, 0, size - 1, compare);
    //free array
    free(array);
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

static size_t get_array(const btree tree, void (*order)(const btree,cvoidp*,int*), voidp** array) {
    if (tree != NULL) {
        size_t size = bt_count(tree);
        voidp* a = (voidp*)malloc(sizeof(voidp*) * size);
        if (array != NULL) {
            int index = 0;
            (*order)(tree, (cvoidp*)a, &index);
            *array = a;
            return size;
        } else {
            errcset(EBTREE_WRITEARR_MEMALLOC);
            return -1;
        }
    } else {
        errcset(EBTREE_WRITEARR_EMPTY);
        return -1;
    }
}

size_t bt_toarray_preorder(const btree tree, voidp** array) {
    if (tree == NULL)
        return 0;
    return get_array(tree, &preorder, array);
}

size_t bt_toarray_inorder(const btree tree, voidp** array) {
    if (tree == NULL)
        return 0;
    return get_array(tree, &inorder, array);
}

size_t bt_toarray_postorder(const btree tree, voidp** array) {
    if (tree == NULL)
        return 0;
    return get_array(tree, &postorder, array);
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
    if (tree == NULL)
        return 0;
    return 1 + bt_count(tree->right) + bt_count(tree->left);
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

void bt_balance(btree* tree, int (*compare)(cvoidp,cvoidp)) {
    if ((*tree) != NULL) {
        voidp* array;
        size_t size = writeSortedToArray((*tree), &array);
        if (array != NULL) {
            int treesize = bt_count(*tree);
            btree new = bt_create_empty();
            insertFromSortedArray(&new, array, 0, treesize - 1, compare);
            if (bt_count(new) == bt_count(*tree) && bt_depth(new) == bt_mindepth(new)) {
                free(array);
                // free only the nodes 
                free_tree(tree);
                *tree = new;
            } else {
                // rollback (free)
                free(array);
                // rollback (free)
                free_tree(&new);
                errcset(EBTREE_BALANCE);
            }
        } else {
            errcset(EBTREE_WRITEARR_MEMALLOC);
        }
    }
}

static void btfree(btree* tree) {
    if ((*tree) != NULL) {
        btfree(&(*tree)->right);
        btfree(&(*tree)->left);
        voidp e = (voidp)(*tree)->element;
        free(e);
        free((*tree));
        (*tree) = NULL;
    }
}

void bt_free(btree* tree) {
    if ((*tree) != NULL) {
        btfree(tree);
    } else {
        errcset(EBTREE_FREENULLPTR);
    }
}

