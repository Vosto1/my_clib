#include "test.h"

static value* vcreate(int chv) {
    value* ch = (value*)malloc(sizeof(value));
    *ch = (char)chv;
    return ch;
}

static int rascii() {
    return rand() % 94 + 32; // ascii character value span
}

static value* createRandomElement() {
    return vcreate(rascii());
}

static value* step(const btree tree, int steps) {
    if (steps == 0)
        return (value*)tree->element;
    int pick = rand() % 2;
    if (pick == 1)
        if (tree->right == NULL)
            return (value*)tree->element;
        else
            return step(tree->right, steps - 1);
    else
        if (tree->left == NULL)
            return (value*)tree->element;
        else
            return step(tree->left, steps - 1);
}

static value* randomElement(const btree tree) {
    if (tree == NULL)
        return NULL;
    int steps = (rand() % bt_count(tree)) / 2;
    return step(tree, steps);
}

// normal bstree
static int normalbstree_compare(cvoidp v1, cvoidp v2) {
    value* c1 = (value*)v1;
    value* c2 = (value*)v2;
    return (int)*c2 - (int)*c1;
}

// left subtree larger values, right subtree smaller values
static int non_normalbstree_compare(cvoidp v1, cvoidp v2) {
    value* c1 = (value*)v1;
    value* c2 = (value*)v2;
    return (int)*c1 - (int)*c2;
}

//// print functions
// inorder tree print
static void pio(const btree bt) {
        if (bt == NULL)
            return;
        pio(bt->left);
        value* ch = (value*)bt->element;
        printf("['%c';%d] ", *ch, (int)*ch);
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

// array print
static void print_array(value** array, size_t size) {
    if (size == 0) {
        printf("{NULL}\n");
        return;
    }
    printf("{");
    for (int i = 0; i < size; i++)
        printf("\'%c\',", *array[i]);
    printf("}\n\n");
}

// checks if the entire tree is traversable until null
// if the tree is not valid this function will continue forever or crash the program
static bool nullcheck(const btree bt) {
    if (bt != NULL)
        return nullcheck(bt->left) && nullcheck(bt->right);
    else
        return true;
}

static bool pch(const btree curr, const btree prev) {
    if (curr != NULL) {
        return curr->parent == prev && pch(curr->left, curr) && pch(curr->right, curr);
    }
    else
        return true;
}

static bool parentcheck(const btree bt) {
    return pch(bt, NULL);
}

// t1 is above (the parent of) t2 in the tree
static bool nintegrity_compar(const btree parent, const btree child) {
    value* p = (value*)parent->element;
    value* c = (value*)child->element;
    int pi = (int)*p, ci = (int)*c;
    if(pi > ci) { // its > because when right child is equal to its parent it is suppsed to return false
        return true; // ^ equals are treated as larger
    } else {
        return false;
    }
}

// only works on a normal bstree (left subtree smaller values, right subtree larger values)
static bool nbstree_integrity_check(const btree tree) {
    if (tree != NULL) {
        if (tree->parent->right == tree) {
            if (!nintegrity_compar(tree->parent, tree)) { // if the parent is smaller than its child (correct if true)
                return true && nbstree_integrity_check(tree->left) && nbstree_integrity_check(tree->right);
            } else {
                return false;
            }
        } else {
            if (nintegrity_compar(tree->parent, tree)) { // if the parent is larger than its child (correct if true)
                return true && nbstree_integrity_check(tree->left) && nbstree_integrity_check(tree->right);
            } else {
                return false;
            }
        }
    }
    else
        return true;
}

// t1 is above (the parent of) t2 in the tree
static bool non_nintegrity_compar(const btree parent, const btree child) {
    value* p = (value*)parent->element;
    value* c = (value*)child->element;
    int pi = (int)*p, ci = (int)*c;
    if(pi >= ci) { // its >= because when right child is equal to its parent it is suppsed to return true
        return true; // ^ equals are treated as smaller I think, dont remember lol
    } else {
        return false;
    }
}

// only works on a non-normal bstree (left subtree larger values, right subtree smaller values)
static bool non_nbstree_integrity_check(const btree tree) {
    if (tree != NULL) {
        if (tree->parent->right == tree) {
            if (!non_nintegrity_compar(tree->parent, tree)) { // if the parent is smaller than its child (correct if false)
                return false;
            } else {
                return true && non_nbstree_integrity_check(tree->left) && non_nbstree_integrity_check(tree->right);
            }
        } else {
            if (non_nintegrity_compar(tree->parent, tree)) { // if the parent is larger than its child (correct if false)
                return false;
            } else {
                return true && non_nbstree_integrity_check(tree->left) && non_nbstree_integrity_check(tree->right);
            }
        }
    }
    else
        return true;
}

static bool integrity_check(const btree tree, bool (*integrity)(const btree tree)) {
    if (tree == NULL)
        return true;
    else
        return (*integrity)(tree->left) && (*integrity)(tree->right);
}

// normal bstree test suit
static bool nbstree_test_suit(const btree tree) {
    bool a = integrity_check(tree, &nbstree_integrity_check);
    bool b = nullcheck(tree);
    bool c = parentcheck(tree);
    return a && b && c;
}

// left subtree larger than right subtree, bstree test suit
static bool non_nbstree_test_suit(const btree tree) {
    bool a = integrity_check(tree, &non_nbstree_integrity_check);
    bool b = nullcheck(tree);
    bool c = parentcheck(tree);
    return a && b && c;
}

// auto tests
// change comparison functions to right side large side to the opposite and test functionality (no mixing!)

#define TYPES 5
#define MOD_MERGE 100
#define NORMAL 1
#define NON_NORMAL 0

void auto_tests(int n, int mod) {
    srand(time(NULL));
    btree tree = bt_create_empty();
    voidp element;
    int conf, nexttests, type, r1, count;
    bool (*integrity)(const btree tree);
    int (*compare)(cvoidp,cvoidp);

    ticks start, end;
    seconds s;

    unsigned int insertion = 0, deletion = 0, find = 0, balance = 0, to_array = 0, merge = 0;

    for (int i = 0; i < n; i++) {
        conf = rand() % 2;
        nexttests = rand() % mod;
        
        if (conf == 1) { // 1 normal 0 non-normal
            integrity = &nbstree_test_suit;
            compare = &normalbstree_compare;
        } else {
            integrity = &non_nbstree_test_suit;
            compare = &non_normalbstree_compare;
        }
        // test sequence start
        start = now();
        for (int j = 0; j < nexttests; j++) {
            type = rand() % TYPES;

            switch (type) {
                case 0: // insert
                    element = createRandomElement();
                    bt_insert(&tree, element, compare);
                    insertion++;
                    break;
                case 1: // remove
                    element = randomElement(tree);
                    bt_remove(&tree, element, compare);
                    deletion++;
                    break;
                case 2: // merge
                    r1 = rand() % MOD_MERGE;
                    btree temp = bt_create_empty();
                    for (int x = 0; x < r1; x++) {
                        element = createRandomElement();
                        bt_insert(&temp, element, compare);
                        insertion++;
                    }
                    int count1 = bt_count(temp);
                    int count2 = bt_count(tree);
                    bt_merge(&tree, &temp, compare);
                    assert((count1 + count2) == bt_count(tree));
                    temp = NULL;
                    merge++;
                    break;
                case 3: // to array
                    count = bt_count(tree);
                    value** arr;
                    value v1, v2;
                    int size = bt_toarray_inorder(tree, (voidp**)&arr);
                    if (conf == NORMAL) {
                        for (int x = 0; x < size - 1; x++) {
                            v1 = *arr[x];
                            v2 = *arr[x + 1];
                            assert(v1 <= v2); // inorder so they can be smaller or equal <=
                        }
                    } else {
                        for (int x = 0; x < size - 1; x++) {
                            v1 = *arr[x];
                            v2 = *arr[x + 1];
                            assert(v1 >= v2); // inorder (reverse) so they can be larger or equal >=
                        }
                    }
                    to_array++;
                    break;
                case 4: // find
                    if (tree != NULL) {
                        element = randomElement(tree);
                        assert(bt_find(tree, element, compare) != NULL);
                    }
                    find++;
                    break;
                case 5: // balance
                    printf("balance\n");
                    bt_balance(&tree, compare);
                    assert(bt_depth(tree) == bt_mindepth(tree));
                    balance++;
                    break;
            }
            // integrity test
            assert((*integrity)(tree));
        }
        end = now();
        bt_free(&tree);
        tree = bt_create_empty();
        // test sequence end
        s = diff(start, end);
        printf("Computed %d operations", nexttests);
        printf(" --> %d insertions, %d deletions, %d merge, %d to array, %d find, %d balance", insertion, deletion, merge, to_array, find, balance);
        printf(" during %f seconds\n", s);
        insertion = 0;
        deletion = 0;
        merge = 0;
        to_array = 0;
        find = 0;
        balance = 0;
    }
}

// test sequence (explicitly test specific edge cases etc.)
#define SIZE 7
#define SIZE2 10
#define SIZE3 5

static void initt(btree* t, size_t size, const char arr[]) {
    for (int x = 0; x < size; x++)
        bt_insert(t, vcreate(arr[x]), &normalbstree_compare);
}

static void initializeTree(btree* tree) {
    static const char ar[SIZE2] = {'w','r','k','d','X','P','L','J','A','2'};
    initt(tree, SIZE2, ar);
}

static void initializeTree1(btree* tree) {
    static const char ar[SIZE3] = {'w','r','k','d','X'};
    initt(tree, SIZE3, ar);
}

static void initializeTree2(btree* tree) {
    static const char ar[] = {'J','2','P','L','A'};
    initt(tree, SIZE3, ar);
}


void test_sequence() {
    voidp rm;
    value* c = vcreate(99);
    btree bt = bt_create_empty();
    assert(bt_is_empty(bt));
    assert(bt_find(bt, (voidp)c, &normalbstree_compare) == NULL);
    bt_remove(&bt, (voidp)c, &normalbstree_compare);
    assert(bt_depth(bt) == 0 && bt_mindepth(bt) == 0);
    bt_balance(&bt, &normalbstree_compare);

    value* a[SIZE];
    int arr[SIZE] = {5,10,1,3,7,19,16};
    for (int i = 0; i < SIZE; i++)
        a[i] = vcreate(arr[i]);
    
    for (int i = 0; i < SIZE; i++)
        bt_insert(&bt, (voidp)a[i], &normalbstree_compare);
    
    assert(bt_count(bt) == SIZE);

    assert(nbstree_test_suit(bt));

    for (int i = 0; i < SIZE; i++)
        assert(bt_find(bt, (voidp)a[i], &normalbstree_compare) != NULL);

    assert(bt_depth(bt) == 4);
    assert(bt_mindepth(bt) == 3);

    rm = bt_remove(&bt, (voidp)&arr[4], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[4]);
    free(rm);

    assert(bt_count(bt) == 6);
    assert(nbstree_test_suit(bt));

    rm = bt_remove(&bt, (voidp)&arr[5], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[5]);
    free(rm);

    assert(bt_count(bt) == 5);
    assert(nbstree_test_suit(bt));

    rm = bt_remove(&bt, (voidp)&arr[0], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[0]);
    free(rm);

    assert(bt_count(bt) == 4);
    assert(nbstree_test_suit(bt));
    
    // make sure the elements are removed
    assert(bt_find(bt, (voidp)&arr[4], &normalbstree_compare) == NULL);
    assert(bt_find(bt, (voidp)&arr[5], &normalbstree_compare) == NULL);
    assert(bt_find(bt, (voidp)&arr[0], &normalbstree_compare) == NULL);

    // remove the rest of the elements
    rm = bt_remove(&bt, (voidp)&arr[1], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[1]);
    free(rm);

    assert(nbstree_test_suit(bt));
    
    rm = bt_remove(&bt, (voidp)&arr[2], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[2]);
    free(rm);

    assert(nbstree_test_suit(bt));
    
    rm = bt_remove(&bt, (voidp)&arr[3], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[3]);
    free(rm);

    assert(nbstree_test_suit(bt));
    
    rm = bt_remove(&bt, (voidp)&arr[6], &normalbstree_compare);
    assert(rm != NULL);
    assert((int)*(value*)rm == arr[6]);
    free(rm);

    assert(nbstree_test_suit(bt));

    assert(bt_is_empty(bt));

    for (int i = 0; i < SIZE; i++)
        assert(bt_find(bt, (voidp)&arr[i], &normalbstree_compare) == NULL);

    
    value* a2[SIZE2];
    int arr2[10] = {32,33,34,35,36,37,38,39,40,41};
    for (int i = 0; i < SIZE2; i++)
        a2[i] = vcreate(arr2[i]);
    
    bt_insert(&bt, (voidp)a2[0], &normalbstree_compare);
    assert(bt_find(bt, (voidp)a2[0], &normalbstree_compare) != NULL);

    assert(nbstree_test_suit(bt));

    for (int i = 1; i < SIZE2; i++)
        bt_insert(&bt, (voidp)a2[i], &normalbstree_compare);

    assert(bt_count(bt) == SIZE2);
    assert(bt_depth(bt) != bt_mindepth(bt));

    assert(nbstree_test_suit(bt));
    
    print_inorder("depth test", bt);

    bt_free(&bt);
    assert(bt_is_empty(bt));
    assert(bt_count(bt) == 0);
    assert(bt_depth(bt) == 0);

    assert(nbstree_test_suit(bt));

    // test the case when a node is not root
    // and removed with two children
    value* v1 = vcreate(32);
    value* v2 = vcreate(43);
    value* v3 = vcreate(31);
    value* v4 = vcreate(39);
    value* v5 = vcreate(38);
    value* v6 = vcreate(37);
    value* v7 = vcreate(40);
    value* v8 = vcreate(45);
    value* v9 = vcreate(61);

    bt_insert(&bt, v1, &normalbstree_compare);
    bt_insert(&bt, v2, &normalbstree_compare);
    bt_insert(&bt, v3, &normalbstree_compare);
    bt_insert(&bt, v4, &normalbstree_compare);
    bt_insert(&bt, v5, &normalbstree_compare);
    bt_insert(&bt, v6, &normalbstree_compare);
    bt_insert(&bt, v7, &normalbstree_compare);
    bt_insert(&bt, v8, &normalbstree_compare);
    bt_insert(&bt, v9, &normalbstree_compare);

    assert(nbstree_test_suit(bt));
    print_inorder("test the case when a node is not root, p1", bt);

    rm = bt_remove(&bt, v4, &normalbstree_compare); // rm 39
    assert(rm != NULL);
    assert(rm == v4);
    free(rm);

    assert(nbstree_test_suit(bt));
    print_inorder("1 remove p2",bt);

    rm = bt_remove(&bt, v2, &normalbstree_compare);  // rm 43
    assert(rm != NULL);
    assert(rm == v2);
    free(rm);

    assert(nbstree_test_suit(bt));
    print_inorder("2 remove p3", bt);

    bt_free(&bt);
    assert(nbstree_test_suit(bt));

    v1 = NULL;
    v2 = NULL;
    v3 = NULL;
    v4 = NULL;
    v5 = NULL;
    v6 = NULL;
    v7 = NULL;
    v8 = NULL;
    v9 = NULL;

    //////test balance, merge, to array
    
    //// merge

    // merge non-empty with empty
    btree t = bt_create_empty();
    initializeTree(&bt);
    bt_merge(&bt, &t, &normalbstree_compare);
    print_inorder("merge non-empty with empty",bt);
    assert(nbstree_test_suit(bt));
    bt_free(&bt);
    bt_free(&t);

    // merge empty with non-empty
    bt = bt_create_empty();
    t = bt_create_empty();
    initializeTree(&t);
    bt_merge(&bt,&t, &normalbstree_compare);

    print_inorder("merge empty with non-empty",bt);
    assert(nbstree_test_suit(bt));
    bt_free(&bt);
    bt_free(&t);

    // merge non-empty with non-empty
    bt = bt_create_empty();
    t = bt_create_empty();
    initializeTree1(&t);
    initializeTree2(&bt);
    bt_merge(&t,&bt, &normalbstree_compare);

    print_inorder("merge non-empty with non-empty",t);
    assert(nbstree_test_suit(bt));
    bt_free(&bt);
    bt_free(&t);

    //// balance

    // b1
    bt = bt_create_empty();
    initializeTree(&bt);
    bt_balance(&bt, &normalbstree_compare);

    print_inorder("balance b1",bt);
    assert(nbstree_test_suit(bt));
    assert(bt_depth(bt) == bt_mindepth(bt));
    bt_free(&bt);

    // b2
    bt = bt_create_empty();
    initializeTree1(&bt);
    bt_balance(&bt, &normalbstree_compare);

    print_inorder("balance b2",bt);
    assert(nbstree_test_suit(bt));
    assert(bt_depth(bt) == bt_mindepth(bt));
    bt_free(&bt);

    // b3
    bt = bt_create_empty();
    initializeTree2(&bt);
    bt_balance(&bt, &normalbstree_compare);

    print_inorder("balance b3",bt);
    assert(nbstree_test_suit(bt));
    assert(bt_depth(bt) == bt_mindepth(bt));
    bt_free(&bt);

    //// to array

    btree toarr = bt_create_empty();
    size_t size;
    value** arr3;
    value* comp;

    value inorder[SIZE3] = {'2','A','J','L','P'};
    value postorder[SIZE3] = {'A','2','L','P','J'};
    value preorder[SIZE3] = {'J','2','A','P','L'};

    initializeTree2(&toarr);
    print_inorder("to array tree", toarr);
    assert(nbstree_test_suit(bt));

    size = bt_toarray_inorder(toarr, (voidp)&arr3);
    assert(size == SIZE3);
    print_array(arr3, size);

    for (int i = 0; i < SIZE3; i++) {
        comp = arr3[i];
        assert(*comp == inorder[i]);
    }


    size = bt_toarray_postorder(toarr, (voidp)&arr3);
    assert(size == SIZE3);
    print_array(arr3, size);

    for (int i = 0; i < SIZE3; i++) {
        comp = arr3[i];
        assert(*comp == postorder[i]);
    }


    size = bt_toarray_preorder(toarr, (voidp)&arr3);
    assert(size == SIZE3);
    print_array(arr3, size);

    for (int i = 0; i < SIZE3; i++) {
        comp = arr3[i];
        assert(*comp == preorder[i]);
    }


    printf("Tests passed.\n");
}