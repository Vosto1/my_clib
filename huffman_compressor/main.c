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

void print_occurances(void* o)
{
    entry *e = (entry *)o;
    printf("[%c,%d]", e->key, e->value);
}

void print_encode_rule(void* o)
{
    encodeRule* e = (encodeRule*)o;
    printf("[");
    printf("%c", e->data);
    printf(",");
    print_bitvector_concise(&e->code);
    printf("]");
}

void print_inorder(huffmantree hft)
{
    if (hft->left != NULL)
    {
        printf("->left\n");
        print_inorder(hft->left);
    }
    entry* e = (entry*)hft->value;
    if (e->branch)
        printf("branch: %d\n", e->value);
    else
        printf("leaf: %d %c\n", e->value, e->key);
    if (hft->right != NULL)
    {
        printf("->right\n");
        print_inorder(hft->right);
    }
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
    print_hashtable(&ht_occurances, &print_occurances);

    huffmantree hft = create_huffman_tree(ht_occurances);
    print_inorder(hft);
    ht_destroy(&ht_occurances);

    hashtable encode_rule_table = huffman_to_hash_dictionary(hft);
    assert(hft_free(&hft));

    print_hashtable(&encode_rule_table, &print_encode_rule);

    assert(ht_free(&encode_rule_table));
    return 0;
}