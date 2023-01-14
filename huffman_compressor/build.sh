#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g ../bitvector/bitvector.c ../btree/btree.c compressor.c ../hash_dictionary/hashtable.c huffman_tree.c main.c node.c ../priority_queue/priority_queue.c ../s_dynamic_array/sdarray.c ../stack/stack.c text_analysis.c ../utils/file.c -lm ../s_heap/s_heap.c  -o prg.out