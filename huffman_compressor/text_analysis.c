#include "text_analysis.h"

#define ALPHABET 26


hashtable letter_occurances(char *string, int strsize)
{
    hashtable ht;
    if (ht_init(&ht, ALPHABET, &node_hash, &node_entry_compare, &node_free_entry_object) != ALPHABET)
    {
        return ht;
    }

    entry *e;
    entry *l;
    for (int i = 0; i < strsize; i++)
    {
        e = node_create_entry(string[i], 1, false);
        l = ht_lookup(&ht, e); // the hash function is run only on the character --> i.e. the look up function checks if the character is already in the ht
        if (l == UNUSED)
        {
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