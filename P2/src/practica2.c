#include <stdio.h>
#include <stdlib.h>

#include "hash-list.h"
#include "word-utils.h"

void create_local_structure(HashList *hl, char *path);

int main()
{
    HashList hl;

    hl_initialize(&hl, 10);

    create_local_structure(&hl, "fitxer.txt");

    hl_print(&hl);
    hl_free(&hl);

    return 0;
}

void create_local_structure(HashList *hl, char *path)
{
    FILE *text;
    FunctionWordSizePtr saveWord;

    saveWord = ^(char *str, int length) {
        int r;
        r = hl_add_word(hl, str, length);
        //printf(" %s [%d]\n", str, length);

        // This is required to avoid memory leaks because the string 
        // passed by is already stored in the list, and we don't have
        // any use for it
        if ( r == 1 ) {
            free(str);
        }
    };

    text = fopen(path, "r");

    wu_get_words(text, saveWord, NULL);

    fclose(text);
}
