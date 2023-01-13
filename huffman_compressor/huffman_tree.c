#include "huffman_tree.h"

huffmantree create_huffman_tree(hashtable occurances)
{
    priorityqueue pq = pq_create_empty();
    pq_init(&pq, 10, &node_compare, &node_free_object);

    darray values = ht_to_array(&occurances);
    uint count = sda_count(&values);
    for (uint i = 0; i < count; i++)
    {

    }
    huffmantree hft;
    return hft;
}