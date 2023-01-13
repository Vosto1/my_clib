#include "./text_analysis.h"

void print(hashtable *ht)
{
    for (int i = 0; i < ht_size(ht); i++)
    {
        if (ht->entries[i] != UNUSED)
        {
            entry *e = (entry *)ht->entries[i];
            printf("index %d: [%c,%d]\n", i, e->k, e->v);
        }
        else
            printf("index %d: UNUSED\n", i);
    }
    printf("\n");
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
    hashtable ht = letter_occurances(d, str);
    print(&ht);
    return 0;
}