#include "bstree.h"

// create a new bstree node
static bstree new_node(cvoidp_t element)
{
    bstree n = (bstree)malloc(sizeof(struct treenode));
    if (n != NULL)
    {
        n->cont.element = element;
        n->cont.next = NULL;
        n->left = NULL;
        n->right = NULL;
        n->parent = NULL;
        return n;
    }
    else
    {
        errcset(EBTREE_NODE_MEMALLOC);
        return NULL;
    }
}

// compare functions

static bool is_smaller(cvoidp_t element1, cvoidp_t element2, int (*compare)(cvoidp_t, cvoidp_t));
static bool is_bigger(cvoidp_t element1, cvoidp_t element2, int (*compare)(cvoidp_t, cvoidp_t));
static bool is_equal(cvoidp_t element1, cvoidp_t element2, int (*compare)(cvoidp_t, cvoidp_t));
static bool left_isnull(const bstree bst);
static bool right_isnull(const bstree bst);

static bool is_smaller(cvoidp_t element1, cvoidp_t element2, int (*compare)(cvoidp_t, cvoidp_t))
{
    return (*compare)(element1, element2) < 0;
}

static bool is_bigger(cvoidp_t element1, cvoidp_t element2, int (*compare)(cvoidp_t, cvoidp_t))
{
    return (*compare)(element1, element2) > 0;
}

static bool is_equal(cvoidp_t element1, cvoidp_t element2, int (*compare)(cvoidp_t, cvoidp_t))
{
    return (*compare)(element1, element2) == 0;
}

static bool left_isnull(const bstree bst)
{
    return bst->left == NULL;
}

static bool right_isnull(const bstree bst)
{
    return bst->right == NULL;
}

bstree bst_create_empty(void)
{
    return NULL;
}

static void insert(bstree *current, bstree prev, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t))
{
    if (*current == NULL)
    {
        *current = new_node(element);
        (*current)->parent = prev;
    }
    else if (is_equal((*current)->cont.element, element, compare))
    {
        datacontainer *new_cont = (datacontainer *)malloc(sizeof(datacontainer));
        new_cont->element = element;
        new_cont->next = NULL;

        datacontainer *tmp = (*current)->cont.next;
        if (tmp != NULL)
        {
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new_cont;
        }
        else
        {
            (*current)->cont.next = new_cont;
        }
    }
    else if (is_bigger((*current)->cont.element, element, compare)) // if tree->e is bigger or equal to e
        insert(&(*current)->right, *current, element, compare);
    else // if tree->e is smaller than e
        insert(&(*current)->left, *current, element, compare);
}

void bst_insert(bstree *tree, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t))
{
    insert(tree, NULL, element, compare);
}

// remove element helper functions

static bstree *find_smallest_right(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t));
static bstree *find_largest_left(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t));
static voidp_t rm_with_no_children(bstree *bst);
static voidp_t rm_with_right_child(bstree *bst);
static voidp_t rm_with_left_child(bstree *bst);
static voidp_t rm_with_two_children(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t));
static voidp_t rm_element_occurance(bstree rm);
static bstree remove_node(bstree *tree, bstree rm, int (*compare)(cvoidp_t, cvoidp_t));
static bstree rm_node_with_no_children(bstree *bst);
static bstree rm_node_with_right_child(bstree *bst);
static bstree rm_node_with_left_child(bstree *bst);
static bstree rm_node_with_two_children(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t));

static bstree *find_smallest_right(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t))
{
    bstree *tmp = &(*bst)->right;
    while (!left_isnull(*tmp))
    {
        tmp = &(*tmp)->left;
    }
    return tmp;
}

static bstree *find_largest_left(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t))
{
    bstree *tmp = &(*bst)->left;
    while (!right_isnull(*tmp))
    {
        tmp = &(*tmp)->right;
    }
    return tmp;
}

static voidp_t rm_element_occurance(bstree rm)
{
    // handle values that already exist in the tree
    datacontainer *tmp = rm->cont.next;
    datacontainer *prev = &rm->cont;
    while (tmp->next != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    datacontainer *new_rm = prev->next;
    prev->next = NULL; // remove pointer to last object of type

    voidp_t element = (voidp_t)new_rm->element;
    free(new_rm);
    return element;
}

static voidp_t rm_with_no_children(bstree *bst)
{
    bstree rm = *bst;

    if (rm->cont.next == NULL)
    { // if the last of its value
        if (rm->parent != NULL)
            if (rm->parent->right == rm)
                rm->parent->right = NULL;
            else
                rm->parent->left = NULL;
        *bst = NULL; // for good measure
        voidp_t e = (voidp_t)rm->cont.element;
        free(rm);
        rm = NULL;
        return e;
    }
    else
        rm_element_occurance(rm);
}

static voidp_t rm_with_right_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->right;
    bstree parent = rm->parent;

    if (rm->cont.next == NULL)
    { // if the last of its value
        if (rm->parent != NULL)
            if (parent->right == rm)
                parent->right = child;
            else
                parent->left = child;

        child->parent = parent;
        (*bst) = child; // for good measure
        voidp_t e = (voidp_t)rm->cont.element;
        free(rm);
        return e;
    }
    else
        rm_element_occurance(rm);
}

static voidp_t rm_with_left_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->left;
    bstree parent = rm->parent;

    if (rm->cont.next == NULL)
    { // if the last of its value
        if (rm->parent != NULL)
            if (parent->right == rm)
                parent->right = child;
            else
                parent->left = child;

        child->parent = parent;
        (*bst) = child; // for good measure
        voidp_t e = (voidp_t)rm->cont.element;
        free(rm);
        return e;
    }
    else
        rm_element_occurance(rm);
}

static voidp_t rm_with_two_children(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t))
{
    bstree rm = *bst;
    bstree *repl;
    // get replacement node
    int random = rand() % 2;
    if (random == 1)
    {
        repl = find_smallest_right(bst, compare);
    }
    else
    {
        repl = find_largest_left(bst, compare);
    }

    // remove node to replace with, and then replace with node to remove with that
    if (rm->cont.next == NULL)
    { // if the last of its value
        bstree node = remove_node(bst, *repl, compare);
        bstree parent = rm->parent;

        bstree lchild = rm->left;
        bstree rchild = rm->right;

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

        (*bst) = node;
        voidp_t e = (voidp_t)rm->cont.element;
        free(rm);
        return e;
    }
    else
        rm_element_occurance(rm);
}

static bstree rm_node_with_no_children(bstree *bst)
{
    bstree rm = *bst;

    if (rm->parent != NULL)
        if (rm->parent->right == rm)
            rm->parent->right = NULL;
        else
            rm->parent->left = NULL;
    *bst = NULL; // for good measure
    return rm;
}

static bstree rm_node_with_right_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->right;
    bstree parent = rm->parent;

    if (rm->parent != NULL)
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;

    child->parent = parent;
    (*bst) = child; // for good measure
    return rm;
}

static bstree rm_node_with_left_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->left;
    bstree parent = rm->parent;

    if (rm->parent != NULL)
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;

    child->parent = parent;
    (*bst) = child; // for good measure
    return rm;
}

static bstree rm_node_with_two_children(bstree *bst, int (*compare)(cvoidp_t, cvoidp_t))
{
    bstree rm = *bst;
    bstree *repl;
    // get replacement node
    int random = rand() % 2;
    if (random == 1)
    {
        repl = find_smallest_right(bst, compare);
    }
    else
    {
        repl = find_largest_left(bst, compare);
    }

    // remove node to replace with, and then replace with node to remove with that
    bstree element = remove_node(bst, (voidp_t)(*repl)->cont.element, compare);
    bstree parent = rm->parent;

    bstree lchild = rm->left;
    bstree rchild = rm->right;

    bstree node = new_node(element);
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

    (*bst) = node; // set the new node at rms' position in the tree
    return rm;
}

static bstree remove_node(bstree *tree, bstree rm, int (*compare)(cvoidp_t, cvoidp_t))
{
    if (*tree != NULL)
    {
        if (rm == *tree) // comparing adresses, because we want to remove a node, not an element.
        {
            if (left_isnull(*tree) && right_isnull(*tree))
            { // if the node doesnt have children
                return rm_node_with_no_children(tree);
            }
            else if (left_isnull(*tree))
            { // if the node has a right child
                return rm_node_with_right_child(tree);
            }
            else if (right_isnull(*tree))
            { // if the node has a left child
                return rm_node_with_left_child(tree);
            }
            else
            { // if the node has two children
                return rm_node_with_two_children(tree, compare);
            }
        }
        else
        { // was not equal
            if (is_bigger((*tree)->cont.element, rm->cont.element, compare)) // find the node by comparing size of elements
            {
                return remove_node(&(*tree)->right, rm, compare);
            }
            else
            {
                return remove_node(&(*tree)->left, rm, compare);
            }
        }
    }
    else
    { // btree doesnt contain element
        return NULL;
    }
}

voidp_t bst_remove(bstree *tree, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t))
{
    if (*tree != NULL)
    {
        if (is_equal((*tree)->cont.element, element, compare))
        {
            if (left_isnull(*tree) && right_isnull(*tree))
            { // if the node doesnt have children
                return rm_with_no_children(tree);
            }
            else if (left_isnull(*tree))
            { // if the node has a right child
                return rm_with_right_child(tree);
            }
            else if (right_isnull(*tree))
            { // if the node has a left child
                return rm_with_left_child(tree);
            }
            else
            { // if the node has two children
                return rm_with_two_children(tree, compare);
            }
        }
        else
        { // tree->element was not equal to element
            if (is_bigger((*tree)->cont.element, element, compare))
            {
                return bst_remove(&(*tree)->right, element, compare);
            }
            else
            {
                return bst_remove(&(*tree)->left, element, compare);
            }
        }
    }
    else
    { // btree doesnt contain element
        return NULL;
    }
}

bool bst_is_empty(const bstree tree)
{
    return tree == NULL;
}

static dim_t writeSortedToArray(const bstree tree, voidp_t **array)
{
    return bst_toarray_inorder(tree, array);
}

static void insertFromSortedArray(bstree *tree, voidp_t *a, int start, int end, int (*compare)(cvoidp_t, cvoidp_t))
{
    if (start > end)
        return;

    int mid = (start + end) / 2;
    bst_insert(tree, a[mid], compare);

    insertFromSortedArray(tree, a, start, mid - 1, compare);
    insertFromSortedArray(tree, a, mid + 1, end, compare);
}

static void free_container_structs(datacontainer *cont)
{
    if (cont != NULL)
    {
        free_container_structs(cont->next);
        free(cont);
        cont = NULL;
    }
}

void bst_merge(bstree *bst1, bstree *bst2, int (*compare)(cvoidp_t, cvoidp_t))
{
    if ((*bst2) == NULL)
        return;
    voidp_t *array;
    dim_t size = writeSortedToArray(*bst2, &array);
    insertFromSortedArray(bst1, array, 0, size - 1, compare);
    // free array
    free(array);
    // free nodes only of bst2
    bst_destroy(bst2);
}

static void preorder(const bstree tree, cvoidp_t *a, int *index)
{
    if (tree != NULL)
    {
        datacontainer *tmp = &tree->cont;
        while (tmp != NULL)
        {
            a[*index] = tmp->element;
            *index += 1;
            tmp = tmp->next;
        }
        if (tree->left != NULL)
            preorder(tree->left, a, index);
        if (tree->right != NULL)
            preorder(tree->right, a, index);
    }
}

static void inorder(const bstree tree, cvoidp_t *a, int *index)
{
    if (tree != NULL)
    {
        if (tree->left != NULL)
            inorder(tree->left, a, index);
        datacontainer *tmp = &tree->cont;
        while (tmp != NULL)
        {
            a[*index] = tmp->element;
            *index += 1;
            tmp = tmp->next;
        }
        if (tree->right != NULL)
            inorder(tree->right, a, index);
    }
}

static void postorder(const bstree tree, cvoidp_t *a, int *index)
{
    if (tree != NULL)
    {
        if (tree->left != NULL)
            postorder(tree->left, a, index);
        if (tree->right != NULL)
            postorder(tree->right, a, index);
        datacontainer *tmp = &tree->cont;
        while (tmp != NULL)
        {
            a[*index] = tmp->element;
            *index += 1;
            tmp = tmp->next;
        }
    }
}

static dim_t get_array(const bstree tree, void (*order)(const bstree, cvoidp_t *, int *), voidp_t **arr)
{
    if (tree != NULL)
    {
        dim_t size = bst_count(tree);
        voidp_t *a = (voidp_t *)malloc(sizeof(voidp_t *) * size);
        if (arr != NULL)
        {
            int index = 0;
            (*order)(tree, (cvoidp_t *)a, &index);
            *arr = a;
            return size;
        }
        else
        {
            errcset(EBTREE_WRITEARR_MEMALLOC);
            return 0;
        }
    }
    else
    {
        errcset(EBTREE_WRITEARR_EMPTY);
        return 0;
    }
}

dim_t bst_toarray_preorder(const bstree tree, voidp_t **array)
{
    if (tree == NULL)
        return 0;
    return get_array(tree, &preorder, array);
}

dim_t bst_toarray_inorder(const bstree tree, voidp_t **array)
{
    if (tree == NULL)
        return 0;
    return get_array(tree, &inorder, array);
}

dim_t bst_toarray_postorder(const bstree tree, voidp_t **array)
{
    if (tree == NULL)
        return 0;
    return get_array(tree, &postorder, array);
}

cvoidp_t bst_find(const bstree tree, voidp_t element, int (*compare)(cvoidp_t, cvoidp_t))
{
    if (tree != NULL)
    {
        if (is_equal(tree->cont.element, element, compare))
        {
            return tree->cont.element;
        }
        else if (is_bigger(tree->cont.element, element, compare))
        {
            return bst_find(tree->right, element, compare);
        }
        else
        {
            return bst_find(tree->left, element, compare);
        }
    }
    else
    { // btree doesnt contain element
        return NULL;
    }
}

cvoidp_t bst_rightmost(const bstree *tree, int (*compare)(cvoidp_t, cvoidp_t))
{
    bstree *tmp = (bstree *)tree;
    while ((*tmp)->right != NULL)
    {
        tmp = &(*tmp)->right;
    }
    return (*tmp)->cont.element;
}

cvoidp_t bst_leftmost(const bstree *tree, int (*compare)(cvoidp_t, cvoidp_t))
{
    bstree *tmp = (bstree *)tree;
    while ((*tmp)->left != NULL)
    {
        tmp = &(*tmp)->left;
    }
    return (*tmp)->cont.element;
}

int bst_count(const bstree tree)
{
    if (tree == NULL)
        return 0;
    int x = 0;
    datacontainer *tmp = &tree->cont;
    do
    {
        x++;
        tmp = tmp->next;
    } while (tmp != NULL);
    return x + bst_count(tree->right) + bst_count(tree->left);
}

int bst_node_count(const bstree tree)
{
    if (tree == NULL)
        return 0;
    return 1 + bst_node_count(tree->right) + bst_node_count(tree->left);
}

int bst_depth(const bstree tree)
{
    int x = 1, y = 1;
    if (tree == NULL)
        return 0;
    x += bst_depth(tree->left);
    y += bst_depth(tree->right);
    if (x >= y)
        return x;
    else
        return y;
}

int bst_mindepth(const bstree tree)
{
    if (tree != NULL)
        return (int)ceil(log((double)bst_node_count(tree) + 1.0f) / log(2.0f));
    return 0;
}

#define INIT_SIZE 10
void bst_balance(bstree *tree, int (*compare)(cvoidp_t, cvoidp_t))
{
    if ((*tree) != NULL)
    {

        // write all elements to an array
        voidp_t *arr1;
        dim_t arraysize = writeSortedToArray((*tree), &arr1);
        if (arr1 != NULL)
        {
            bstree new = bst_create_empty();
            sdarray arr2 = sda_create_empty();
            if (sda_init(&arr2, INIT_SIZE) != 0)
            {
                // get an array with unique elements
                voidp_t tmp = arr1[0];
                sda_insert(&arr2, tmp);
                arr1[0] = NULL;
                for (int i = 1; i < arraysize; i++)
                {
                    if (!is_equal(tmp, arr1[i], compare))
                    {
                        sda_insert(&arr2, arr1[i]);
                        tmp = arr1[i];
                        arr1[i] = NULL;
                    }
                }

                // insert unique elements in the tree balanced
                insertFromSortedArray(&new, arr2.array, 0, arr2.used - 1, compare);
                sda_destroy(&arr2); // free array and not the elements

                // add all the non-unique elements
                for (int i = 0; i < arraysize; i++)
                    if (arr1[i] != NULL)
                        bst_insert(&new, arr1[i], compare);
            }
            int nc = bst_count(new);
            int oc = bst_count(*tree);
            int d = bst_depth(new);
            int md = bst_mindepth(new);
            // was it successful?
            if (nc == oc && d == md)
            {
                free(arr1);
                // free only the nodes
                bst_destroy(tree);
                *tree = new;
            }
            else
            {
                // rollback (free)
                free(arr1);
                // rollback (free)
                bst_destroy(&new);
                errcset(EBTREE_BALANCE);
            }
        }
        else
        {
            errcset(EBTREE_WRITEARR_MEMALLOC);
        }
    }
}

static void free_all_elements(datacontainer *cont)
{
    if (cont->next != NULL)
        free_all_elements(cont->next);
    free((voidp_t)cont->element);
}

static void bstfree(bstree *tree)
{
    if ((*tree) != NULL)
    {
        bstfree(&(*tree)->right);
        bstfree(&(*tree)->left);
        // free all elements in the node
        datacontainer *tmp = &(*tree)->cont;
        free_all_elements(tmp);
        // free all value structs in which the elements were stored
        datacontainer *tmp2 = (*tree)->cont.next;
        free_container_structs(tmp2);
        // free the node
        free((*tree));
        (*tree) = NULL;
    }
}

void bst_free(bstree *tree)
{
    if ((*tree) != NULL)
    {
        bstfree(tree);
    }
    else
    {
        errcset(EBTREE_FREENULLPTR);
    }
}

void bstdestr(bstree *tree)
{
    if ((*tree) != NULL)
    {
        bstdestr(&(*tree)->right);
        bstdestr(&(*tree)->left);
        // free all data container structs in which the elements were stored
        datacontainer *tmp = (*tree)->cont.next;
        free_container_structs(tmp);
        // free node
        free((*tree));
        (*tree) = NULL;
    }
}

// free only nodes (and data container structs) (not elements in the tree)
void bst_destroy(bstree *tree)
{
    if ((*tree) != NULL)
    {
        bstdestr(tree);
    }
    else
    {
        errcset(EBTREE_FREENULLPTR);
    }
}