#include "btree.h"

static void print(voidp e) {
    char* c = (char*)e;
    printf("%c",*c);
}

static btree new_node(cvoidp element) {
    btree n = (btree)malloc(sizeof(struct treenode));
    if (n != NULL) {
        n->cont.element = element;
        n->cont.next = NULL;
        n->left = NULL;
        n->right = NULL;
        n->parent = NULL;
        return n;
    } else { 
        errcset(EBTREE_NODE_MEMALLOC);
        return NULL;
    }
}

/* static bool is_bigger_or_equal(cvoidp element1, cvoidp element2, int (*compare)(cvoidp,cvoidp)) {
    return (*compare)(element1, element2) >= 0;
} */

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

static void insert(btree* current, btree prev, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (*current == NULL) {
        *current = new_node(element);
        (*current)->parent = prev;
    }
    else if (is_equal((*current)->cont.element, element, compare)) {
        datacontainer* new_cont = (datacontainer*)malloc(sizeof(datacontainer));
        new_cont->element = element;
        new_cont->next = NULL;

        datacontainer* tmp = (*current)->cont.next;
        if (tmp != NULL) {
            while (tmp->next != NULL)
                tmp = tmp->next;
            tmp->next = new_cont;
        } else {
            (*current)->cont.next = new_cont;
        }
    }
    else if (is_bigger((*current)->cont.element, element, compare)) // if tree->e is bigger or equal to e
        insert(&(*current)->right, *current, element, compare);
    else // if tree->e is smaller than e
        insert(&(*current)->left, *current, element, compare);
}

void bt_insert(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    insert(tree, NULL, element, compare);
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

    if (rm->cont.next == NULL) { // if the last of its value
        if (rm->parent != NULL)
            if (rm->parent->right == rm)
                rm->parent->right = NULL;
            else
                rm->parent->left = NULL;
        *bt = NULL;  // for good measure
        voidp e = (voidp)rm->cont.element;
        free(rm);
        rm = NULL;
        return e;
    } else { // handle values that already exist in the tree
        datacontainer* tmp = rm->cont.next;
        datacontainer* prev = &rm->cont;
        while (tmp->next != NULL) {
            prev = tmp;
            tmp = tmp->next;
        }
        datacontainer* new_rm = prev->next;
        prev->next = NULL; // remove pointer to last object of type

        voidp element = (voidp)new_rm->element;
        free(new_rm);
        return element;
    }
}

static voidp rmWithRightChild(btree* bt) {
    btree rm = *bt;
    btree child = rm->right;
    btree parent = rm->parent;

    if (rm->cont.next == NULL) { // if the last of its value
        if (rm->parent != NULL)
            if (parent->right == rm)
                parent->right = child;
            else
                parent->left = child;

        child->parent = parent;
        (*bt) = child; // for good measure
        voidp e = (voidp)rm->cont.element;
        free(rm);
        return e;
    } else { // handle values that already exist in the tree
        datacontainer* tmp = rm->cont.next;
        datacontainer* prev = &rm->cont;
        while (tmp->next != NULL) {
            prev = tmp;
            tmp = tmp->next;
        }
        datacontainer* new_rm = prev->next;
        prev->next = NULL; // remove pointer to last object of type

        voidp element = (voidp)new_rm->element;
        free(new_rm);
        return element;
    }
}

static voidp rmWithLeftChild(btree* bt) {
    btree rm = *bt;
    btree child = rm->left;
    btree parent = rm->parent;

    if (rm->cont.next == NULL) { // if the last of its value
        if (rm->parent != NULL)
            if (parent->right == rm)
                parent->right = child;
            else
                parent->left = child;
        
        child->parent = parent;
        (*bt) = child; // for good measure
        voidp e = (voidp)rm->cont.element;
        free(rm);
        return e;
    } else { // handle values that already exist in the tree
        datacontainer* tmp = rm->cont.next;
        datacontainer* prev = &rm->cont;
        while (tmp->next != NULL) {
            prev = tmp;
            tmp = tmp->next;
        }
        datacontainer* new_rm = prev->next;
        prev->next = NULL; // remove pointer to last object of type

        voidp element = (voidp)new_rm->element;
        free(new_rm);
        return element;
    }
}

static voidp rmWithTwoChildren(btree* bt, int (*compare)(cvoidp,cvoidp)) {
    btree rm = *bt;
    btree* repl;
    // get replacement node
    int random = rand() % 2;
    if (random == 1) {
        repl = findSmallestNodeRight(bt, compare);
    } else {
        repl = findLargestNodeLeft(bt, compare);
    }

    // remove node to replace with and replace with node to remove
    if (rm->cont.next == NULL) { // if the last of its value
        voidp element = bt_remove(bt, (voidp)(*repl)->cont.element, compare);
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
        voidp e = (voidp)rm->cont.element;
        free(rm);
        return e;
    } else { // handle values that already exist in the tree
        datacontainer* tmp = rm->cont.next;
        datacontainer* prev = &rm->cont;
        while (tmp->next != NULL) {
            prev = tmp;
            tmp = tmp->next;
        }
        datacontainer* new_rm = prev->next;
        prev->next = NULL; // remove pointer to last object of type

        voidp element = (voidp)new_rm->element;
        free(new_rm);
        return element;
    }
}

voidp bt_remove(btree* tree, voidp element, int (*compare)(cvoidp,cvoidp)) {
    if (*tree != NULL) {
        if (is_equal((*tree)->cont.element, element, compare)) {
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
            if (is_bigger((*tree)->cont.element, element, compare)) {
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

static void free_container_structs(datacontainer* cont) {
    if (cont != NULL) {
        free_container_structs(cont->next);
        free(cont);
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
    bt_destroy(bt2);
}

static void preorder(const btree tree, cvoidp* a, int* index) {
    if (tree != NULL) {
        datacontainer* tmp = &tree->cont;
        while(tmp != NULL) {
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

static void inorder(const btree tree, cvoidp* a, int* index) {
    if (tree != NULL) {
        if (tree->left != NULL)
            inorder(tree->left, a, index);
        datacontainer* tmp = &tree->cont;
        while(tmp != NULL) {
            a[*index] = tmp->element;
            *index += 1;
            tmp = tmp->next;
        }
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
        datacontainer* tmp = &tree->cont;
        while(tmp != NULL) {
            a[*index] = tmp->element;
            *index += 1;
            tmp = tmp->next;
        }
    }
}

static size_t get_array(const btree tree, void (*order)(const btree,cvoidp*,int*), voidp** arr) {
    if (tree != NULL) {
        size_t size = bt_count(tree);
        voidp* a = (voidp*)malloc(sizeof(voidp*) * size);
        if (arr != NULL) {
            int index = 0;
            (*order)(tree, (cvoidp*)a, &index);
            *arr = a;
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
        if (is_equal(tree->cont.element, element, compare)) {
            return tree->cont.element;
        } else if (is_bigger(tree->cont.element, element, compare)) {
            return bt_find(tree->right, element, compare);
        } else {
            return bt_find(tree->left, element, compare);
        }
    } else { // btree doesnt contain element
        return NULL;
    }
}

cvoidp bt_rightmost(const btree* tree, int (*compare)(cvoidp,cvoidp)) {
    btree* tmp = (btree*)tree;
    while ((*tmp)->right != NULL) {
        tmp = &(*tmp)->right;
    }
    return (*tmp)->cont.element;
}

cvoidp bt_leftmost(const btree* tree, int (*compare)(cvoidp,cvoidp)) {
    btree* tmp = (btree*)tree;
        while ((*tmp)->left != NULL) {
            tmp = &(*tmp)->left;
        }
        return (*tmp)->cont.element;
}

int bt_count(const btree tree) {
    if (tree == NULL)
        return 0;
    int x = 0;
    datacontainer* tmp = &tree->cont;
    do {
        x++;
        tmp = tmp->next;
    } while (tmp != NULL);
    return x + bt_count(tree->right) + bt_count(tree->left);
}

int bt_node_count(const btree tree) {
    if (tree == NULL)
        return 0;
    return 1 + bt_node_count(tree->right) + bt_node_count(tree->left);
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
        return (int)ceil(log((double)bt_node_count(tree) + 1.0f) / log(2.0f));
    return 0;
}

static void pio(const btree bt) {
        if (bt == NULL)
            return;
        pio(bt->left);
        datacontainer* tmp = &bt->cont;
        while(tmp != NULL) {
            char* ch = (char*)tmp->element;
            printf("['%c';%d] ", *ch, (int)*ch);
            tmp = tmp->next;
        }
        pio(bt->right);
}

static void print_inorder(const char* description, const btree bt) {
    if (bt != NULL) {
        if (description != NULL)
            printf("%s ", description);
        printf("(format: [char;(int)char])\n");
        pio(bt);
        printf("\n\n");
    }
}

#define INIT_SIZE 10
void bt_balance(btree* tree, int (*compare)(cvoidp,cvoidp)) {
    if ((*tree) != NULL) {

        // write all elements to an array
        voidp* arr1;
        size_t arraysize = writeSortedToArray((*tree), &arr1);
        if (arr1 != NULL) {
            btree new = bt_create_empty();
            s_array arr2 = sda_createEmpty();
            if (sda_init(&arr2, INIT_SIZE) != -1) {
                // get an array with unique elements
                voidp tmp = arr1[0];
                sda_insert(&arr2, tmp);
                arr1[0] = NULL;
                for (int i = 1; i < arraysize; i++) {
                    if (!is_equal(tmp, arr1[i], compare)) {
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
                        bt_insert(&new, arr1[i], compare);
            }
            int nc = bt_count(new);
            int oc = bt_count(*tree);
            int d = bt_depth(new);
            int md = bt_mindepth(new);
            // was it successful?
            if (nc == oc && d == md) {
                free(arr1);
                // free only the nodes 
                bt_destroy(tree);
                *tree = new;
            } else {
                for (int i = 0; i < arraysize; i++)
                    if (arr1[i] != NULL)
                        printf("[%c,%d] ", *(char*)arr1[i], (int)*(char*)arr1[i]);
                    else
                        printf("[NULL] ");
                printf("\n\n\n");
                print_inorder("error t", *tree);
                print_inorder("error n", new);
                // rollback (free)
                free(arr1);
                // rollback (free)
                bt_destroy(&new);
                errcset(EBTREE_BALANCE);
            }
        } else {
            errcset(EBTREE_WRITEARR_MEMALLOC);
        }
    }
}

static void free_all_elements(datacontainer* cont) {
    if (cont->next != NULL)
        free_all_elements(cont->next);
    free((voidp)cont->element);
}

static void btfree(btree* tree) {
    if ((*tree) != NULL) {
        btfree(&(*tree)->right);
        btfree(&(*tree)->left);
        // free all elements in the node
        datacontainer* tmp = &(*tree)->cont;
        free_all_elements(tmp);
        // free all value structs in which the elements were stored
        datacontainer* tmp2 = (*tree)->cont.next;
        free_container_structs(tmp2);
        // free the node
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

void btdestr(btree* tree) {
    if ((*tree) != NULL) {
        btdestr(&(*tree)->right);
        btdestr(&(*tree)->left);
        // free all data container structs in which the elements were stored
        datacontainer* tmp = (*tree)->cont.next;
        free_container_structs(tmp);
        // free node
        free((*tree));
        (*tree) = NULL;
    }
}

// free only nodes (and data container structs) (not elements in the tree)
void bt_destroy(btree* tree) {
    if ((*tree) != NULL) {
        btdestr(tree);
    } else {
        errcset(EBTREE_FREENULLPTR);
    }
}