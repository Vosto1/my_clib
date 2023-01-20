#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <assert.h>
#include "../btree/btree.h"
#include "../hash_dictionary/hashtable.h"
#include "../priority_queue/priority_queue.h"
#include "../string_builder/stringbuilder.h"
#include "../s_dynamic_array/sdarray.h"
#include "../dynamic_string/dstring.h"
#include "../bitvector/bitvector.h"
#include "text_analysis.h"
#include "../stack/stack.h"
#include "node.h"

typedef btree huffmantree;
huffmantree hft_create(hashtable occurances);
hashtable hft_to_dictionary(huffmantree hft);
bool hft_free(huffmantree* hft);
bitvector hft_to_binary(huffmantree hft);
huffmantree hft_binary_to_huffmantree(bitvector* binary, uint* index_out);
void hft_print_inorder(huffmantree hft);
void encode(dstring filename);

#endif