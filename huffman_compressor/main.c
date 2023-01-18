#include "./text_analysis.h"
#include "huffman_tree.h"
#include "node.h"

void print_hashtable(hashtable *ht, void (*print_item)(void* o))
{
    for (int i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            printf("index %d: ", i);
            (*print_item)(ht->entries[i]);
            printf("\n");
        }
        else
            printf("index %d: UNUSED\n", i);
    }
    printf("\n");
}

void print_string(char* str, uint len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c", str[i]);
    }
    printf("\n");
}

int main(void)
{
    char *file_contents;
    uint len = read_text_file("textAnalysis.txt", &file_contents);
    print_string(file_contents, len);
    /* char* a = (char*)malloc(sizeof(char) * 10);
    int b = 32;
    char c;
    for (int i = 0; i < 10; i++) {
        c = (char)(i + b);
        a[i] = c;
    } */
    hashtable ht_occurances = letter_occurances(file_contents, len);
    free(file_contents);
    print_hashtable(&ht_occurances, &node_print_entry);

    huffmantree hft = hft_create(ht_occurances);
    hft_print_inorder(hft);
    ht_destroy(&ht_occurances);

    hashtable encode_rule_table = hft_to_dictionary(hft);
    bitvector hftvector = hft_to_binary(hft);
    binary hftbin;
    bools2bits(&hftvector, &hftbin);
    print_binary(&hftbin);

    assert(hft_free(&hft));

    print_hashtable(&encode_rule_table, &encode_rule_print);

    assert(ht_free(&encode_rule_table));
    return 0;
}