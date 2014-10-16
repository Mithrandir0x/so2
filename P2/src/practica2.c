#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash-list.h"
#include "red-black-tree.h"
#include "word-utils.h"

/**
 *
 * @param hl   A HashList pointer where to pour all the words available from a file
 * @param path A string indicating the path to the file to be parsed
 */
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
    if ( !text )
    {
        printf("El fitxer [%s] no existeix o no es pot obrir.\n", path);
        return;
    }

    wu_get_words(text, saveWord, NULL);

    fclose(text);
}

/**
 *
 * @param tree     Pointer to the Red-Black Tree structure that saves all the word counts for each file
 * @param file_num File number where the file comes from
 * @param word     Pointer to the word to be stored in the r-b tree
 * @param n        Times it appeared in a file
 */
void update_global_tree_node(RBTree *tree, int file_num, char *word, int n)
{
    int k;
    char *copy;
    RBData *data;

    data = findNode(tree, word);

    if ( data != NULL )
    {
        k = data->tpf[file_num];
        data->tpf[file_num] += n;
        data->total++;
    }
    else
    {
        data = malloc(sizeof(RBData));
        
        // We don't want to reference the string passed by, so we copy the string
        // so when the tree must be freed, it will not try to free a string referenced
        // in another place.
        //
        // Each piece of data in its own place should be managed by its owner.
        copy = malloc(sizeof(char) * ( strlen(word) + 1 ));
        strcpy(copy, word);
        
        data->primary_key = copy;
        
        data->tpf = malloc(sizeof(int) * ( tree->num_files ));
        data->tpf[file_num] = n;
        data->num_files = tree->num_files;
        
        data->total = n;

        insertNode(tree, data);
    }
}

/**
 *
 * @param tree      Pointer to the Red-Black Tree structure that saves all the word counts for each file
 * @param hl        Pointer to the HashList that stores all the words from a file
 * @param files_num An index of the file being parsed
 */
void update_global_structure(RBTree *tree, HashList *hl, int file_num)
{
    int n;
    char *word;
    HashListIterator* iter;
    List *list;
    ListItem *item;
    ListData *data;

    iter = hl_iterator(hl);
    while ( ( list = hl_next(iter) ) != NULL )
    {
        item = list->first;
        while ( item != NULL )
        {
            data = item->data;

            word = data->primary_key;
            n = data->numTimes;

            update_global_tree_node(tree, file_num, word, n);

            item = item->next;
        }
    }

    free(iter);
}

int main()
{
    HashList hl;
    RBTree tree;

    hl_initialize(&hl, 10);
    initTree(&tree, 1);

    create_local_structure(&hl, "fitxer.txt");
    update_global_structure(&tree, &hl, 0);

    hl_print(&hl);
    printTree(&tree);
    
    hl_free(&hl);
    deleteTree(&tree);

    return 0;
}
