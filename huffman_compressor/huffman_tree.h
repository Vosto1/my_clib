#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "../btree/btree.h"
#include "../hash_dictionary/hashtable.h"
#include "../priority_queue/priority_queue.h"
#include "../dynamic_array/darray.h"
#include "node.h"

typedef btree huffmantree;
huffmantree create_huffman_tree(hashtable occurances);

#endif