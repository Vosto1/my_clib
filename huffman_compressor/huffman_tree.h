#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <assert.h>
#include "../btree/btree.h"
#include "../hash_dictionary/hashtable.h"
#include "../priority_queue/priority_queue.h"
#include "../s_dynamic_array/sdarray.h"
#include "../bitvector/bitvector.h"
#include "../stack/stack.h"
#include "node.h"

typedef btree huffmantree;
huffmantree hft_create(hashtable occurances);
hashtable hft_to_dictionary(huffmantree hft);
bool hft_free(huffmantree* hft);
bitvector hft_to_binary(huffmantree hft);
void hft_print_inorder(huffmantree hft);

#endif