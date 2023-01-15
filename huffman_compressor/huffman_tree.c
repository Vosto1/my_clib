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

huffmantree create_huffman_tree(hashtable occurances)
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

hashtable huffman_to_hash_dictionary(huffmantree hft)
{
    hashtable ht = ht_create_empty();
    bitvector code = bv_create_empty();
    stack s = st_create_empty();

    int size = 10;
    assert(ht_init(&ht, size, &encode_rule_hash, &encode_rule_compare, &encode_rule_free));
    assert(bv_init(&code));
    assert(st_init(&s, size, &node_free_object));

    encodeRule* er = NULL;
    entry* e = NULL;
    btree current = hft;
    btree nil = node_create(NULL); // dummy node
    btree previous = nil;
    bitvector copy = bv_create_empty();
    while (!(st_is_empty(&s) && current == NULL))
    {
        while (current != NULL)
        {
            assert(st_push(&s, current));
            bv_add(&code, false);
            current = current->left;
        }

        current = st_pop(&s);
        bv_remove_last(&code);
        e = (entry*)current->value;
        if (!e->branch)
        {
            copy = bv_duplicate(&code);
            er = encode_rule_create(e->key, copy);
            ht_insert(&ht, er);
        }
        if (previous != current->right)
        {
            st_push(&s, current);
            bv_add(&code, true);
            current = current->right;
        }
        else
        {
            st_push(&s, current);
            bv_add(&code, true);
            current = NULL;
        }
        if (current == NULL) // if we went to the right and immediately hit NULL or backtracking toward root again
        {
            previous = st_pop(&s);
            bv_remove_last(&code);
        }
    }
    bv_delete(&code);
    st_free(&s);
    free(nil);
    return ht;
}

bool hft_free(huffmantree* hft)
{
    return bt_free((btree*)hft, &node_free_entry_object);
}