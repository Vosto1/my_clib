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

int main(void)
{
    char *d;
    size_t str = read_text_file("textAnalysis.txt", &d);
    for (int i = 0; i < str; i++)
    {
        printf("%c", d[i]);
    }
    printf("\n");
    /* char* a = (char*)malloc(sizeof(char) * 10);
    int b = 32;
    char c;
    for (int i = 0; i < 10; i++) {
        c = (char)(i + b);
        a[i] = c;
    } */
    hashtable ht_occurances = letter_occurances(d, str);
    print(&ht_occurances);

    huffmantree hft = create_huffman_tree(ht_occurances);
    print_inorder(hft);

    return 0;
}