#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g ../btree/btree.c compressor.c ../hash_dictionary/hashtable.c huffman_tree.c main.c node.c ../dynamic_array/darray.c ../s_heap/s_heap.c ../priority_queue/priority_queue.c ../s_dynamic_array/sdarray.c text_analysis.c ../utils/file.c  -o prg.out