#include "test.h"

//// help funcs

static item* vcreate(int chv) {
    item* ch = (item*)malloc(sizeof(item));
    *ch = (char)chv;
    return ch;
}

static int rascii() {
    return rand() % 94 + 32; // ascii character value span
}

static item* createRandomElement() {
    return vcreate(rascii());
}

static item* step(const btree tree, int steps) {
    if (steps == 0)
        return (item*)tree->cont.element;
    int pick = rand() % 2;
    if (pick == 1)
        if (tree->right == NULL)
            return (item*)tree->cont.element;
        else
            return step(tree->right, steps - 1);
    else
        if (tree->left == NULL)
            return (item*)tree->cont.element;
        else
            return step(tree->left, steps - 1);
}

static item* randomElement(const btree tree) {
    if (tree == NULL)
        return NULL;
    int steps = (rand() % bt_count(tree)) / 2;
    return step(tree, steps);
}

//// print functions
// inorder tree print
static void pio(const btree bt) {
        if (bt == NULL)
            return;
        pio(bt->left);
        datacontainer* tmp = &bt->cont;
        while(tmp != NULL) {
            item* ch = (item*)tmp->element;
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

// array print
static void print_array(item** array, size_t size) {
    if (size == 0) {
        printf("{NULL}\n");
        return;
    }
    printf("{");
    for (int i = 0; i < size; i++)
        printf("\'%c\',", *array[i]);
    printf("}\n\n");
}


//// compare and test funcs

// normal bstree compare (left subtree smaller, right subtree larger)
static int compare(cvoidp v1, cvoidp v2) {
    item* c1 = (item*)v1;
    item* c2 = (item*)v2;
    return (int)*c2 - (int)*c1;
}

// checks if the entire tree is traversable until null
// if the tree is not valid this function will continue forever or crash the program
static bool nullcheck(const btree bt) {
    if (bt != NULL)
        return nullcheck(bt->left) && nullcheck(bt->right);
    else
        return true;
}

// checks that all nodes has the correct parent
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

// parent is above the child in the hierarchy of the tree
static bool integrity_compar(const btree parent, const btree child, bool (*comp)(int,int)) {
    item *p, *c;
    int pi, ci;
    bool sum = true;
    datacontainer* ptmp = &parent->cont;
    datacontainer* ctmp = &child->cont;
    // make sure that all elements (with the same value) of the parent is
    // larger than all the elements (same values) of the child
    while (ctmp->next != NULL && ptmp->next != NULL) {
        p = (voidp)ptmp->element;
        pi = (int)*p;
        c = (voidp)ctmp->element;
        ci = (int)*c;
        sum &= (*comp)(pi, ci); // sum bools
        ptmp = ptmp->next;
        ctmp = ctmp->next;
    }
    return sum;
}

static bool right(int p, int c) {
    return p < c;
}

static bool left(int p, int c) {
    return p > c;
}

// only works on a normal bstree (left subtree smaller values, right subtree larger values)
static bool bstree_integrity_check(const btree tree) {
    if (tree != NULL) {
        if (tree->parent->right == tree) { // right side (parent should be larger)
            if (integrity_compar(tree->parent, tree, &right)) {
                return true && bstree_integrity_check(tree->left) && bstree_integrity_check(tree->right);
            } else {
                return false;
            }
        } else { // left side (parent should be smaller)
            if (integrity_compar(tree->parent, tree, &left)) {
                return true && bstree_integrity_check(tree->left) && bstree_integrity_check(tree->right);
            } else {
                return false;
            }
        }
    }
    else
        return true;
}


static bool integrity_check(const btree tree) {
    if (tree == NULL)
        return true;
    else
        return bstree_integrity_check(tree->left) && bstree_integrity_check(tree->right);
}

// normal bstree test suit
static bool bstree_test_suit(const btree tree) {
    bool a = integrity_check(tree);
    bool b = nullcheck(tree);
    bool c = parentcheck(tree);
    if (!a)
        printf("integrity fail\n");
    if (!b)
        printf("null fail\n");
    if (!c)
        printf("parent fail\n");
    return a && b && c;
}

// auto tests
// change comparison functions to right side large side to the opposite and test functionality (no mixing!)

#define TYPES 5
#define MOD_MERGE 100

void auto_tests(int n, int mod) {
    srand(time(NULL));
    errcinit();
    btree tree = bt_create_empty();
    voidp element, rm;
    int nexttests, type, r1, count;
    bool (*integrity)(const btree tree);
    int (*comp)(cvoidp,cvoidp);
    comp = &compare;

    ticks start, end;
    seconds s;

    unsigned int insertion = 0, deletion = 0, find = 0, balance = 0, to_array = 0, merge = 0;

    for (int i = 0; i < n; i++) {
        nexttests = rand() % mod;

        // test sequence start
        start = now();
        for (int j = 0; j < nexttests; j++) {
            type = rand() % TYPES + 1;

            switch (type) {
                case 0: // insert
                    element = createRandomElement();
                    bt_insert(&tree, element, comp);
                    assert(bt_find(tree, element, compare) != NULL);
                    insertion++;
                    break;
                case 1: // remove
                    if (tree != NULL) {
                        element = randomElement(tree);
                        rm = bt_remove(&tree, element, comp);
                        assert(rm != NULL);
                        assert(compare(element, rm) == 0);
                        free(rm);
                        element = NULL;
                        rm = NULL;
                        deletion++;
                    }
                    break;
                case 2: // merge
                    r1 = rand() % MOD_MERGE;
                    btree temp = bt_create_empty();
                    for (int x = 0; x < r1; x++) {
                        element = createRandomElement();
                        bt_insert(&temp, element, comp);
                        insertion++;
                    }
                    int count1 = bt_count(temp);
                    int count2 = bt_count(tree);
                    bt_merge(&tree, &temp, comp);
                    assert((count1 + count2) == bt_count(tree));
                    temp = NULL;
                    merge++;
                    break;
                case 3: // to array
                    if (tree != NULL) {   
                        count = bt_count(tree);
                        item** arr;
                        item *v1, *v2;
                        int size = bt_toarray_inorder(tree, (voidp**)&arr);
                        assert(count == size);
                        for (int x = 0; x < size - 1; x++) {
                            v1 = arr[x];
                            v2 = arr[x + 1];
                            assert(*v1 <= *v2); // inorder so they can be smaller or equal <=
                        }
                        free(arr);
                        to_array++;
                    }
                    break;
                case 4: // find
                    if (tree != NULL) {
                        element = randomElement(tree);
                        assert(bt_find(tree, element, comp) != NULL);
                    }
                    find++;
                    break;
                case 5: // balance
                    bt_balance(&tree, comp);
                    assert(bt_depth(tree) == bt_mindepth(tree));
                    balance++;
                    break;
            }
            // integrity test
            if (!bstree_test_suit(tree)) {
                printf("latest operation type: ");
                switch (type)
                {
                case 0:
                    printf("insert");
                    break;
                case 1:
                    printf("remove");
                    break;
                case 2:
                    printf("merge");
                    break;
                case 3:
                    printf("to array");
                    break;
                case 4:
                    printf("find");
                    break;
                case 5:
                    printf("balance");
                    break;
                default:
                    printf("unknown");
                    break;
                }
                printf("\n\n");
                print_inorder("error", tree);
                exit(-1);
            }
        }
        end = now();
        errorHandler();
        if (tree != NULL)
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
#define SIZE4 3

static void initt(btree* t, size_t size, const char arr[]) {
    for (int x = 0; x < size; x++)
        bt_insert(t, vcreate(arr[x]), &compare);
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

// multiple elements with the same value inserted into the tree
static void initializeTree3(btree* tree) {
    static const char ar[] = {'J','2','P'};
    initt(tree, SIZE4, ar);
    initt(tree, SIZE4, ar);
    initt(tree, SIZE4, ar);
}


void test_sequence() {
    voidp rm;
    item* c = vcreate(99);
    btree bt = bt_create_empty();
    assert(bt_is_empty(bt));
    assert(bt_find(bt, (voidp)c, &compare) == NULL);
    bt_remove(&bt, (voidp)c, &compare);
    assert(bt_depth(bt) == 0 && bt_mindepth(bt) == 0);
    bt_balance(&bt, &compare);

    item* a[SIZE];
    int arr[SIZE] = {5,10,1,3,7,19,16};
    for (int i = 0; i < SIZE; i++)
        a[i] = vcreate(arr[i]);
    
    for (int i = 0; i < SIZE; i++)
        bt_insert(&bt, (voidp)a[i], &compare);
    
    assert(bt_count(bt) == SIZE);

    assert(bstree_test_suit(bt));

    for (int i = 0; i < SIZE; i++)
        assert(bt_find(bt, (voidp)a[i], &compare) != NULL);

    assert(bt_depth(bt) == 4);
    assert(bt_mindepth(bt) == 3);

    rm = bt_remove(&bt, (voidp)&arr[4], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[4]);
    free(rm);

    assert(bt_count(bt) == 6);
    assert(bstree_test_suit(bt));

    rm = bt_remove(&bt, (voidp)&arr[5], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[5]);
    free(rm);

    assert(bt_count(bt) == 5);
    assert(bstree_test_suit(bt));

    rm = bt_remove(&bt, (voidp)&arr[0], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[0]);
    free(rm);

    assert(bt_count(bt) == 4);
    assert(bstree_test_suit(bt));
    
    // make sure the elements are removed
    assert(bt_find(bt, (voidp)&arr[4], &compare) == NULL);
    assert(bt_find(bt, (voidp)&arr[5], &compare) == NULL);
    assert(bt_find(bt, (voidp)&arr[0], &compare) == NULL);

    // remove the rest of the elements
    rm = bt_remove(&bt, (voidp)&arr[1], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[1]);
    free(rm);

    assert(bstree_test_suit(bt));
    
    rm = bt_remove(&bt, (voidp)&arr[2], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[2]);
    free(rm);

    assert(bstree_test_suit(bt));
    
    rm = bt_remove(&bt, (voidp)&arr[3], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[3]);
    free(rm);

    assert(bstree_test_suit(bt));
    
    rm = bt_remove(&bt, (voidp)&arr[6], &compare);
    assert(rm != NULL);
    assert((int)*(item*)rm == arr[6]);
    free(rm);

    assert(bstree_test_suit(bt));

    assert(bt_is_empty(bt));

    for (int i = 0; i < SIZE; i++)
        assert(bt_find(bt, (voidp)&arr[i], &compare) == NULL);

    
    item* a2[SIZE2];
    int arr2[10] = {32,33,34,35,36,37,38,39,40,41};
    for (int i = 0; i < SIZE2; i++)
        a2[i] = vcreate(arr2[i]);
    
    bt_insert(&bt, (voidp)a2[0], &compare);
    assert(bt_find(bt, (voidp)a2[0], &compare) != NULL);

    assert(bstree_test_suit(bt));

    for (int i = 1; i < SIZE2; i++)
        bt_insert(&bt, (voidp)a2[i], &compare);

    assert(bt_count(bt) == SIZE2);
    assert(bt_depth(bt) != bt_mindepth(bt));

    assert(bstree_test_suit(bt));
    
    print_inorder("depth test", bt);

    bt_free(&bt);
    assert(bt_is_empty(bt));
    assert(bt_count(bt) == 0);
    assert(bt_depth(bt) == 0);

    assert(bstree_test_suit(bt));

    // test the case when a node is not root
    // and removed with two children
    item* v1 = vcreate(32);
    item* v2 = vcreate(43);
    item* v3 = vcreate(31);
    item* v4 = vcreate(39);
    item* v5 = vcreate(38);
    item* v6 = vcreate(37);
    item* v7 = vcreate(40);
    item* v8 = vcreate(45);
    item* v9 = vcreate(61);

    bt_insert(&bt, v1, &compare);
    bt_insert(&bt, v2, &compare);
    bt_insert(&bt, v3, &compare);
    bt_insert(&bt, v4, &compare);
    bt_insert(&bt, v5, &compare);
    bt_insert(&bt, v6, &compare);
    bt_insert(&bt, v7, &compare);
    bt_insert(&bt, v8, &compare);
    bt_insert(&bt, v9, &compare);

    assert(bstree_test_suit(bt));
    print_inorder("test the case when a node is not root, p1", bt);

    rm = bt_remove(&bt, v4, &compare); // rm 39
    assert(rm != NULL);
    assert(rm == v4);
    free(rm);

    assert(bstree_test_suit(bt));
    print_inorder("1 remove p2",bt);

    rm = bt_remove(&bt, v2, &compare);  // rm 43
    assert(rm != NULL);
    assert(rm == v2);
    free(rm);

    assert(bstree_test_suit(bt));
    print_inorder("2 remove p3", bt);

    bt_free(&bt);
    assert(bstree_test_suit(bt));

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
    bt_merge(&bt, &t, &compare);
    print_inorder("merge non-empty with empty",bt);
    assert(bstree_test_suit(bt));
    bt_free(&bt);
    bt_free(&t);

    // merge empty with non-empty
    bt = bt_create_empty();
    t = bt_create_empty();
    initializeTree(&t);
    bt_merge(&bt,&t, &compare);

    print_inorder("merge empty with non-empty",bt);
    assert(bstree_test_suit(bt));
    bt_free(&bt);
    bt_free(&t);

    // merge non-empty with non-empty
    bt = bt_create_empty();
    t = bt_create_empty();
    initializeTree1(&t);
    initializeTree2(&bt);
    bt_merge(&t,&bt, &compare);

    print_inorder("merge non-empty with non-empty",t);
    assert(bstree_test_suit(bt));
    bt_free(&bt);
    bt_free(&t);

    //// balance

    // b1
    bt = bt_create_empty();
    initializeTree(&bt);
    bt_balance(&bt, &compare);

    print_inorder("balance b1",bt);
    assert(bstree_test_suit(bt));
    assert(bt_depth(bt) == bt_mindepth(bt));
    bt_free(&bt);

    // b2
    bt = bt_create_empty();
    initializeTree1(&bt);
    bt_balance(&bt, &compare);

    print_inorder("balance b2",bt);
    assert(bstree_test_suit(bt));
    assert(bt_depth(bt) == bt_mindepth(bt));
    bt_free(&bt);

    // b3
    bt = bt_create_empty();
    initializeTree2(&bt);
    bt_balance(&bt, &compare);

    print_inorder("balance b3",bt);
    assert(bstree_test_suit(bt));
    assert(bt_depth(bt) == bt_mindepth(bt));
    bt_free(&bt);

    //// to array

    btree toarr = bt_create_empty();
    size_t size;
    item** arr3;
    item* comp;

    item inorder[SIZE3] = {'2','A','J','L','P'};
    item postorder[SIZE3] = {'A','2','L','P','J'};
    item preorder[SIZE3] = {'J','2','A','P','L'};

    initializeTree2(&toarr);
    print_inorder("to array tree", toarr);
    assert(bstree_test_suit(bt));

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

    // test adding multiple items with the same value
    bt = bt_create_empty();

    v1 = vcreate(32);
    v2 = vcreate(32);
    v3 = vcreate(32);
    v4 = vcreate(39);
    v5 = vcreate(39);
    v6 = vcreate(39);
    v7 = vcreate(41);
    v8 = vcreate(41);
    v9 = vcreate(41);

    bt_insert(&bt, v1, &compare);
    bt_insert(&bt, v2, &compare);
    bt_insert(&bt, v3, &compare);
    bt_insert(&bt, v4, &compare);
    bt_insert(&bt, v5, &compare);
    bt_insert(&bt, v6, &compare);
    bt_insert(&bt, v7, &compare);
    bt_insert(&bt, v8, &compare);
    bt_insert(&bt, v9, &compare);

    assert(bt_count(bt) == 9);
    assert(bstree_test_suit(bt));

    bt_balance(&bt, compare);
    int d = bt_depth(bt);
    int md = bt_mindepth(bt);
    int nc = bt_node_count(bt);
    assert(d == md);
    assert(bt_find(bt, v1, compare));
    assert(bt_find(bt, v5, compare));
    assert(bt_find(bt, v7, compare));

    item c1 = *v1;
    item c5 = *v5;
    item c7 = *v7;
    for (int i = 0; i < 3; i++) {
        rm = bt_remove(&bt, (voidp)&c1, compare);
        assert(rm != NULL);
        assert(compare(rm, (voidp)&c1) == 0);
        free(rm);
    }
    
    for (int i = 0; i < 3; i++) {
        rm = bt_remove(&bt, (voidp)&c5, compare);
        assert(rm != NULL);
        assert(compare(rm, (voidp)&c5) == 0);
        free(rm);
    }

    for (int i = 0; i < 3; i++) {
        rm = bt_remove(&bt, (voidp)&c7, compare);
        assert(rm != NULL);
        assert(compare(rm, (voidp)&c7) == 0);
        free(rm);
    }

    v1 = NULL;
    v2 = NULL;
    v3 = NULL;
    v4 = NULL;
    v5 = NULL;
    v6 = NULL;
    v7 = NULL;
    v8 = NULL;
    v9 = NULL;

    bt_free(&bt);

    printf("Tests passed.\n");
}