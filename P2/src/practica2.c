#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "hash-list.h"
#include "red-black-tree.h"
#include "word-utils.h"

/**
 * Given a path to a text file, this function parses this file's words and stores them
 * inside the HashList given by reference.
 *
 * @param hl   A HashList pointer where to pour all the words available from a file
 * @param path A string indicating the path to the file to be parsed
 */
int create_local_structure(HashList *hl, char *path)
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
        return 1;
    }

    wu_get_words(text, saveWord, NULL);

    fclose(text);

    return 0;
}

/**
 * Given a word, and the number of times it has appeared in a file (specified by its number), store that
 * information in the global structure.
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
        
        // Allocate required memory into heap and initialize it to zero
        data->tpf = calloc(tree->num_files, sizeof(int));
        data->tpf[file_num] = n;
        
        data->num_files = tree->num_files;
        
        data->total = 1;

        insertNode(tree, data);
    }
}

/**
 * Given a local structure that stores the words, and the times they appeared in a file (specified by
 * its number), add them to the global structure.
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

int main(int argc, char **argv)
{
    // "__block" is required to notify the compiler that those variables may be
    // value-assigned by a block instance, not just for read-only purposes.
    __block int initializedTree = 0;
    __block RBTree tree;
    __block HashList hl;
    
    ProgressPtr process_file;
    
    // For each file indicated by the file specified by argument, it will parse
    // its content, searching for words, and it will store them in the global
    // structure.
    process_file = ^(char *file_path, int file_num, int total_files){
        int result;

        printf("Reading file [%s] with id [%d/%d]\n", file_path, file_num + 1, total_files);
        
        if ( !initializedTree )
        {
            initTree(&tree, total_files);
            initializedTree = 1;
        }
        
        hl_initialize(&hl, 10);
        
        result = create_local_structure(&hl, file_path);
        if ( result == 0 )
        {
            update_global_structure(&tree, &hl, file_num);
            //hl_print(&hl);
            hl_free(&hl);
        }
    };
    
    if ( argc != 2 )
    {
        printf("usage: practica2 files.cfg\n");
        exit(1);
    }
    
    cfg_get_file_list(argv[1], 100, process_file);
    
    printTree(&tree);
    
    deleteTree(&tree);

    return 0;
}
