#include "test.h"

void test(const char* string)
{
    void* colors[] = {pcred, pcyellow, pcgreen, pccyan, pcblue, pcpurple};
    int amt_colors = sizeof(colors)/sizeof(colors[0]);
    dstring sentence = ds_new_string_initialize(string);

    for (int i = 0; i < sentence.length; i++)
    {
        void (*fnptr)();
        fnptr = colors[i%amt_colors];
        (*fnptr)();
        printf("%c", ds_at(sentence, i));
        pcreset();
    }
    printf("\n");

    //printf("Tests passed\n");
}