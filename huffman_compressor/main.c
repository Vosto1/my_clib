#include "./text_analysis.h"
#include "huffman_tree.h"
#include "node.h"

void print(hashtable *ht)
{
    for (int i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            entry *e = (entry *)ht->entries[i];
            printf("index %d: [%c,%d]\n", i, e->key, e->value);
        }
        else
            printf("index %d: UNUSED\n", i);
    }
    printf("\n");
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
    //print(&ht_occurances);

    huffmantree hft = create_huffman_tree(ht_occurances);
    //print_inorder(hft);
    ht_destroy(&ht_occurances);

    assert(hft_free(&hft));
    return 0;
}