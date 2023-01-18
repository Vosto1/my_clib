#include "text_analysis.h"

#define ALPHABET 26


hashtable letter_occurances(char *string, uint len)
{
    hashtable ht = ht_create_empty();
    if (ht_init(&ht, ALPHABET, &node_hash, &node_entry_compare, &node_free_entry_object) != ALPHABET)
    {
        return ht;
    }

    entry tmp;
    tmp.branch = false; // the hashtable checks the character only
    tmp.value = 0; // the hashtable checks the character only

    entry *e;
    entry *l;
    for (int i = 0; i < len; i++)
    {
        tmp.key = string[i]; // use variable on the stack in order to avoid creating new memory for each search. This is possible because
        l = ht_lookup(&ht, &tmp); // the hash function is run only on the character --> i.e. the look up function checks if the character is a key already in the ht
        if (l == UNUSED)
        {
            e = node_create_entry(string[i], 1, false); // the node entry will be contained in the huffman tree node
            ht_insert(&ht, e); // if the letter did not exist add to ht
        }
        else
        {
            l->value += 1; // if the letter existed, increment value
        }
    }
    ht_trim(&ht);

    return ht;
}