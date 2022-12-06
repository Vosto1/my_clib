#include "bstree.h"


// create a new bstree node
static bstree new_node(const void * element)
{
    bstree n = (bstree)malloc(sizeof(struct treeNode));
    if (n == NULL)
    {
        
    }

    n->cont.element = element;
    n->cont.next = NULL;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    return n;
}

// compare functions

//static bool is_smaller(const void * element1, const void * element2, int (*compare)(const void *, const void *));
static bool is_bigger(const void * element1, const void * element2, int (*compare)(const void *, const void *));
static bool is_equal(const void * element1, const void * element2, int (*compare)(const void *, const void *));
static bool left_isnull(const bstree bst);
static bool right_isnull(const bstree bst);

/*static bool is_smaller(const void * element1, const void * element2, int (*compare)(const void *, const void *))
{
    return (*compare)(element1, element2) < 0;
}*/

static bool is_bigger(const void * element1, const void * element2, int (*compare)(const void *, const void *))
{
    return (*compare)(element1, element2) > 0;
}

static bool is_equal(const void * element1, const void * element2, int (*compare)(const void *, const void *))
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
    //srand(time(NULL)); // messes with the users rand use (ask user to use srand on their own instead)
    return NULL;
}

static void insert(bstree *current, bstree prev, void* element, int (*compare)(const void *, const void *))
{
    if (*current == NULL)
    {
        *current = new_node(element);
        (*current)->parent = prev;
    }
    else if (is_equal((*current)->cont.element, element, compare))
    {
        dataContainer *new_cont = (dataContainer *)malloc(sizeof(dataContainer));
        new_cont->element = element;
        new_cont->next = NULL;

        dataContainer *tmp = (*current)->cont.next;
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

void bst_insert(bstree *tree, void* element, int (*compare)(const void *, const void *))
{
    insert(tree, NULL, element, compare);
}

// remove element helper functions

static bstree *find_smallest_right(bstree *bst, int (*compare)(const void *, const void *));
static bstree *find_largest_left(bstree *bst, int (*compare)(const void *, const void *));
static void* rm_element_occurance(bstree rm);
static void* rm_with_no_children(bstree *bst);
static void* rm_with_right_child(bstree *bst);
static void* rm_with_left_child(bstree *bst);
static void* rm_with_two_children(bstree *bst, int (*compare)(const void *, const void *));
static bstree rm_node_with_no_children(bstree *bst);
static bstree rm_node_with_right_child(bstree *bst);
static bstree rm_node_with_left_child(bstree *bst);
static bstree rm_node_with_two_children(bstree *bst, int (*compare)(const void *, const void *));
static bstree remove_node(bstree *tree, bstree rm, int (*compare)(const void *, const void *));

static bstree *find_smallest_right(bstree *bst, int (*compare)(const void *, const void *))
{
    bstree *tmp = &(*bst)->right;
    while (!left_isnull(*tmp))
    {
        tmp = &(*tmp)->left;
    }
    return tmp;
}

static bstree *find_largest_left(bstree *bst, int (*compare)(const void *, const void *))
{
    bstree *tmp = &(*bst)->left;
    while (!right_isnull(*tmp))
    {
        tmp = &(*tmp)->right;
    }
    return tmp;
}

static void* rm_element_occurance(bstree rm)
{
    // handle values that already exist in the tree
    dataContainer *tmp = rm->cont.next;
    dataContainer *prev = &rm->cont;
    while (tmp->next != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    dataContainer *new_rm = prev->next;
    prev->next = NULL; // remove pointer to last object of type

    void* element = (void*)new_rm->element;
    free(new_rm);
    return element;
}

static void* rm_with_no_children(bstree *bst)
{
    bstree rm = *bst;

    if (rm->cont.next != NULL)
    { // not the last of its value 
        return rm_element_occurance(rm);
    }
        
    // if the last of its value
    if (rm->parent != NULL)
    {
        if (rm->parent->right == rm)
            rm->parent->right = NULL;
        else
            rm->parent->left = NULL;
    }
    *bst = NULL;
    void* e = (void*)rm->cont.element;
    free(rm);
    rm = NULL;
    return e;
}

static void* rm_with_right_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->right;
    bstree parent = rm->parent;

    if (rm->cont.next != NULL)
    { // not the last of its value
       return rm_element_occurance(rm);
    }

    // if the last of its value
    if (rm->parent != NULL)
    {
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;
    }

    child->parent = parent;
    (*bst) = child;
    void* e = (void*)rm->cont.element;
    free(rm);
    return e;
}

static void* rm_with_left_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->left;
    bstree parent = rm->parent;

    if (rm->cont.next != NULL)
    { // not the last of its value
       return rm_element_occurance(rm);
    } 

    // if the last of its value
    if (rm->parent != NULL)
    {
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;
    }

    child->parent = parent;
    (*bst) = child;
    void* e = (void*)rm->cont.element;
    free(rm);
    return e;
}

static void* rm_with_two_children(bstree *bst, int (*compare)(const void *, const void *))
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

    // remove node-to-replace-with from tree, and then replace node-to-remove with that node

    if (rm->cont.next != NULL)
    { // not last of its value
        return rm_element_occurance(rm);
    }

    // if the last of its value
    bstree node = remove_node(bst, *repl, compare);
    bstree parent = rm->parent;

    bstree lchild = rm->left;
    bstree rchild = rm->right;

    if (lchild != NULL)
        lchild->parent = node;
    if (rchild != NULL)
        rchild->parent = node;

    if (parent != NULL)
    {
        if (parent->right == rm)
            parent->right = node;
        else
            parent->left = node;
    }

    node->left = lchild;
    node->right = rchild;
    node->parent = rm->parent;

    (*bst) = node;
    void* e = (void*)rm->cont.element;
    free(rm);
    return e;
}

static bstree rm_node_with_no_children(bstree *bst)
{
    bstree rm = *bst;

    if (rm->parent != NULL)
    {
        if (rm->parent->right == rm)
            rm->parent->right = NULL;
        else
            rm->parent->left = NULL;
    }
    *bst = NULL;
    return rm;
}

static bstree rm_node_with_right_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->right;
    bstree parent = rm->parent;

    if (rm->parent != NULL)
    {
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;
    }

    child->parent = parent;
    (*bst) = child;
    return rm;
}

static bstree rm_node_with_left_child(bstree *bst)
{
    bstree rm = *bst;
    bstree child = rm->left;
    bstree parent = rm->parent;

    if (rm->parent != NULL)
    {
        if (parent->right == rm)
            parent->right = child;
        else
            parent->left = child;
    }

    child->parent = parent;
    (*bst) = child;
    return rm;
}

static bstree rm_node_with_two_children(bstree *bst, int (*compare)(const void *, const void *))
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
    bstree element = remove_node(bst, (void*)(*repl)->cont.element, compare);
    bstree parent = rm->parent;

    bstree lchild = rm->left;
    bstree rchild = rm->right;

    bstree node = new_node(element);
    if (lchild != NULL)
        lchild->parent = node;
    if (rchild != NULL)
        rchild->parent = node;

    if (parent != NULL)
    {
        if (parent->right == rm)
            parent->right = node;
        else
            parent->left = node;
    }

    node->left = lchild;
    node->right = rchild;
    node->parent = rm->parent;

    (*bst) = node; // set the new node at rms' position in the tree
    return rm;
}

static bstree remove_node(bstree *tree, bstree rm, int (*compare)(const void *, const void *))
{
    if (*tree == NULL)
    {
        return NULL;
    }

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

void* bst_remove(bstree *tree, void* element, int (*compare)(const void *, const void *))
{
    if (*tree == NULL)
    {
        return NULL;
    }

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

bool bst_is_empty(const bstree tree)
{
    return tree == NULL;
}

static size_t write_sorted_to_array(const bstree tree, void* **array)
{
    return bst_toarray_inorder(tree, array);
}

static void insert_from_sorted_array(bstree *tree, void* *a, int start, int end, int (*compare)(const void *, const void *))
{
    if (start > end)
        return;

    int mid = (start + end) / 2;
    bst_insert(tree, a[mid], compare);

    insert_from_sorted_array(tree, a, start, mid - 1, compare);
    insert_from_sorted_array(tree, a, mid + 1, end, compare);
}

static void free_container_structs(dataContainer *cont)
{
    if (cont == NULL)
    {
        return;
    }

    free_container_structs(cont->next);
    free(cont);
    cont = NULL;
}

bstree bst_merge(bstree *bst1, bstree *bst2, int (*compare)(const void *, const void *))
{
    if ((*bst2) != NULL)
    {
        void* *array;
        size_t size = write_sorted_to_array(*bst2, &array);
        insert_from_sorted_array(bst1, array, 0, size - 1, compare);
        // free array
        free(array);
        // free nodes only of bst2
        bst_destroy(bst2);
    }

    // fake that a new tree has been made.
    bstree bst3 = *bst1;
    *bst1 = NULL;
    return bst3;
    // Q: why fake it?
    // A: to avoid the possibility of the user accidentally using a NULL pointer,
    //    and because we dont want to actually create a new tree (optimization).
}

static void preorder(const bstree tree, const void * *a, int *index)
{
    if (tree == NULL)
    {
        return;
    }

    dataContainer *tmp = &tree->cont;
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

static void inorder(const bstree tree, const void * *a, int *index)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->left != NULL)
        inorder(tree->left, a, index);
    dataContainer *tmp = &tree->cont;
    while (tmp != NULL)
    {
        a[*index] = tmp->element;
        *index += 1;
        tmp = tmp->next;
    }
    if (tree->right != NULL)
        inorder(tree->right, a, index);
}

static void postorder(const bstree tree, const void * *a, int *index)
{
    if (tree == NULL)
    {
        return;
    }

    if (tree->left != NULL)
        postorder(tree->left, a, index);
    if (tree->right != NULL)
        postorder(tree->right, a, index);
    dataContainer *tmp = &tree->cont;
    while (tmp != NULL)
    {
        a[*index] = tmp->element;
        *index += 1;
        tmp = tmp->next;
    }
}

static size_t get_array(const bstree tree, void (*order)(const bstree, const void * *, int *), void* **arr)
{
    size_t size = bst_count(tree);
    void* *a = (void* *)malloc(sizeof(void* *) * size);

    if (arr == NULL)
    {
        free(a);
        return 0;
    }

    int index = 0;
    (*order)(tree, (const void * *)a, &index);
    *arr = a;
    return size;
}

size_t bst_toarray_preorder(const bstree tree, void* **array)
{
    if (tree == NULL)
        return 0;
    return get_array(tree, &preorder, array);
}

size_t bst_toarray_inorder(const bstree tree, void* **array)
{
    if (tree == NULL)
        return 0;
    return get_array(tree, &inorder, array);
}

size_t bst_toarray_postorder(const bstree tree, void* **array)
{
    if (tree == NULL)
        return 0;
    return get_array(tree, &postorder, array);
}

const void * bst_find(const bstree tree, void* element, int (*compare)(const void *, const void *))
{
    if (tree == NULL)
    {
        return NULL;
    }

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

const void * bst_rightmost(const bstree *tree, int (*compare)(const void *, const void *))
{
    bstree *tmp = (bstree *)tree;
    while ((*tmp)->right != NULL)
    {
        tmp = &(*tmp)->right;
    }
    return (*tmp)->cont.element;
}

const void * bst_leftmost(const bstree *tree, int (*compare)(const void *, const void *))
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
    dataContainer *tmp = &tree->cont;
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

#define INIT_SIZE 100
bool bst_balance(bstree *tree, int (*compare)(const void *, const void *))
{
    if ((*tree) == NULL)
    {
        return false;
    }

    // write all elements to an array
    void* *arr1;
    size_t arraysize = write_sorted_to_array((*tree), &arr1);
    if (arr1 == NULL)
    {
        return false;
    }
    bstree new = bst_create_empty();
    sdarray arr2 = sda_create_empty();
    if (sda_init(&arr2, INIT_SIZE, NULL) != 0)
    {
        // get an array with unique elements
        void* tmp = arr1[0];
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
        insert_from_sorted_array(&new, arr2.array, 0, arr2.used - 1, compare);
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
        return true;
    }
    else
    {
        // rollback (free)
        free(arr1);
        // rollback (free)
        bst_destroy(&new);
        //errcset(EBTREE_BALANCE);
        return false;
    }
}

static void free_all_elements(dataContainer *cont, void (*freeObject)(void*))
{
    if (cont->next != NULL)
        free_all_elements(cont->next, freeObject);
    (*freeObject)((void*)cont->element);
}

static void bstfree(bstree *tree, void (*freeObject)(void*))
{
    if ((*tree) == NULL)
    {
        return;
    }
    bstfree(&(*tree)->right, freeObject);
    bstfree(&(*tree)->left, freeObject);
    // free all elements in the node
    dataContainer *tmp = &(*tree)->cont;
    free_all_elements(tmp, freeObject);
    // free all value structs in which the elements were stored
    dataContainer *tmp2 = (*tree)->cont.next;
    free_container_structs(tmp2);
    // free the node
    free((*tree));
    (*tree) = NULL;
}

bool bst_free(bstree *tree, void (*freeObject)(void*))
{
    if ((*tree) == NULL)
    {
        return false;
    }
    bstfree(tree, freeObject);
    return true;
}

void bstdestr(bstree *tree)
{
    if ((*tree) == NULL)
    {
        return;
    }
    bstdestr(&(*tree)->right);
    bstdestr(&(*tree)->left);
    // free all data container structs in which the elements were stored
    dataContainer *tmp = (*tree)->cont.next;
    free_container_structs(tmp);
    // free node
    free((*tree));
    (*tree) = NULL;
}

// free only nodes (and data container structs, not elements in the tree)
bool bst_destroy(bstree *tree)
{
    if ((*tree) == NULL)
    {
        return false;
    }
    bstdestr(tree);
    return true;
}