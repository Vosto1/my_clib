#include "huffman_tree.h"

/*static void heapPrintTree(sheap *h)
{
    int y = 0;
    int x = 0;
    for (int i = 0; i < sh_size(h); i++)
    {
        for (int j = 0; j < pow(2.0, (double)i) && j + pow(2.0, (double)i) <= sh_size(h); j++)
        {
            x = j + (int)pow(2.0, (double)i) - 1;
            y = h->items.used;
            if (y > x)
            {
                node n = (node)h->items.array[x];
                entry* e = (entry*)n->value;
                printf("[k%c|%d]", e->key, e->value);
            }
            else
                printf("----");
        }
        printf("\n");
    }
}*/

static node merge(node n1, node n2);

huffmantree hft_create(hashtable occurances)
{
    uint size = 10;
    priorityqueue pq = pq_create_empty();
    assert(pq_init(&pq, size, &node_compare, &node_free_object) == size);

    sdarray values = ht_to_array(&occurances);
    uint count = sda_count(&values);
    for (uint i = 0; i < count; i++)
    {
        node n = node_create((void*)sda_at(&values, i));
        pq_enqueue(&pq, n);
    }
    assert(sda_destroy(&values));


    //heapPrintTree(&pq.h);
    for (uint i = 0; i < count; i++)
    {
        node n1 = pq_dequeue(&pq);
        node n2 = pq_dequeue(&pq);
        node new = merge(n1, n2);
        pq_enqueue(&pq, new);
        //heapPrintTree(&pq.h);
    }
    huffmantree hft = (huffmantree)pq_dequeue(&pq);
    assert(pq_destroy(&pq));
    return hft;
}

static node merge(node n1, node n2)
{
    if (n1 == NULL)
    {
        return n2;
    }
    if (n2 == NULL)
    {
        return n1;
    }
    entry* e1 = (entry*)n1->value;
    entry* e2 = (entry*)n2->value;
    node new = node_create(node_create_entry((char)0, e1->value + e2->value, true));
    if (e1->value >= e2->value)
    {
        new->left = n1;
        new->right = n2;
    }
    else
    {
        new->left = n2;
        new->right = n1;
    }
    return new;
}

hashtable hft_to_dictionary(huffmantree hft)
{
    hashtable ht = ht_create_empty();
    bitvector code = bv_create_empty();
    stack nodes = st_create_empty();

    int size = 10;
    assert(ht_init(&ht, size, &encode_rule_hash, &encode_rule_compare, &encode_rule_free) == size);
    assert(bv_init(&code));
    assert(st_init(&nodes, size, &node_free_object) == size);

    encodeRule* er = NULL;
    entry* e = NULL;
    btree current = hft;
    btree nil = node_create(NULL); // dummy node
    btree previous = nil; // to not screw up the first check if the first current->right is equal to null (then previous cannot be equal to null)
    bitvector copy = bv_create_empty();
    while (!(st_is_empty(&nodes) && current == NULL))
    {
        while (current != NULL) // go to the node farthest to the left from current
        {
            assert(st_push(&nodes, current));
            bv_add(&code, false);
            current = current->left;
        }

        current = st_pop(&nodes); // pop that node
        bv_remove_last(&code);
        e = (entry*)current->value;
        if (!e->branch) // if its not a branch then add it to the ht
        {
            copy = bv_duplicate(&code);
            er = encode_rule_create(e->key, copy);
            ht_insert(&ht, er);
        }
        if (previous != current->right) // if true then we already visited the right tree and need to backtrack
        {
            st_push(&nodes, current);
            bv_add(&code, true);
            current = current->right;
        }
        else // backtrack -> re-push node and bit that was popped
        {
            st_push(&nodes, current);
            bv_add(&code, true);
            current = NULL; // so that we can backtrack
        }
        if (current == NULL) // if we went to the right and immediately hit NULL or if we want to be backtracking toward root
        {
            previous = st_pop(&nodes);
            bv_remove_last(&code);
        }
    }
    bv_delete(&code);
    st_free(&nodes);
    free(nil);
    return ht;
}

bool hft_free(huffmantree* hft)
{
    return bt_free((btree*)hft, &node_free_entry_object);
}

bitvector hft_to_binary(huffmantree hft)
{
    stack nodes = st_create_empty();
    bitvector hftbinary = bv_create_empty();

    int size = 10;
    assert(st_init(&nodes, size, &node_free_object) == size);
    assert(bv_init(&hftbinary));

    entry* e;
    btree current = hft;
    assert(st_push(&nodes, current));
    while (!st_is_empty(&nodes))
    {
        current = st_pop(&nodes);
        e = (entry*)current->value;
        if (!e->branch)
        {
            bv_add(&hftbinary, false);
            bool b;
            for (uint i = 0; i < 8; i++)
            {
                b = (bool)(e->key & (1 << i));
                bv_add(&hftbinary, b);
            }
        }
        else
        {
            bv_add(&hftbinary, true);
        }
        if (current->right != NULL)
            assert(st_push(&nodes, current->right));
        if(current->left != NULL)
            assert(st_push(&nodes, current->left));
    }

    st_free(&nodes);
    return hftbinary;
}

huffmantree hft_binary_to_huffmantree(bitvector* binary)
{
    huffmantree tree_to_build = node_create(NULL);
    stack nodes;
    int index = 0; 

    int size = 10;
    assert(st_init(&nodes, size, &node_free_object) == size);


    huffmantree current = tree_to_build;
    bool* tmp;
    byte b = (byte)0;

    st_push(&nodes, current);

    while (!st_is_empty(&nodes))
    {
        current = st_pop(&nodes);
        tmp = bv_at(binary, index);
        index++;
        if (*tmp)
        {
            current->value = node_create_entry((char)0, -1, true); // don't need occurances anymore, set to -1
            // since it was a branch it has to have 2 children, hence:
            current->left = node_create(NULL);
            current->right = node_create(NULL);
            st_push(&nodes, current->right);
            st_push(&nodes, current->left);
        }
        else
        {
            for (int i = 7 + index, j = 7; i >= 0 + index; i--, j--) // byte is backwards (i >= 0 + index to make it clear what it does)
            {
                tmp = bv_at(binary, i);
                if (*tmp)
                {
                    b |= (1 << j); // set bit on pos j (j goes from 7 -> 0)
                }
            }
            index += 8;
            current->value = node_create_entry(b, -1, false); // don't need occurances
            b = (byte)0; // reset
        }
    }
    st_free(&nodes);
    return tree_to_build;
}

void hft_print_inorder(huffmantree hft)
{
    if (hft->left != NULL)
    {
        printf("->left\n");
        hft_print_inorder(hft->left);
    }
    entry* e = (entry*)hft->value;
    if (e->branch)
        printf("branch: %d\n", e->value);
    else
        printf("leaf: %d %c\n", e->value, e->key);
    if (hft->right != NULL)
    {
        printf("->right\n");
        hft_print_inorder(hft->right);
    }
}