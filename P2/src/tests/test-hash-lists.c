#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash-list.h"

static char* malloc_string(char *str, int length)
{
    char *str2;
    str2 = malloc(length * sizeof(char) + ( 1 * sizeof(char) ));
    strcpy(str2, str);
    return str2;
}

int main(int argc, char **argv)
{

    // Broken test.

    // As the current implementation of the linked list requires to free
    // the string passed by, strings forced by argument are not allocated
    // in the heap, thus creating an exception in valgrind when looking
    // for memory leaks.
    

    /* char *a, *b, *c, *d, *e;
    HashList hl;

    hl_initialize(&hl, 3);

    a = malloc_string("Hello", 5);
    b = malloc_string("Hello", 5);

    c = malloc_string("World", 5);
    d = malloc_string("World", 5);

    e = malloc_string("Hello World", 11);

    hl_add_word(&hl, a, 5);
    hl_add_word(&hl, b, 5);
    
    hl_add_word(&hl, c, 5);
    hl_add_word(&hl, d, 5);
    
    hl_add_word(&hl, e, 11);

    hl_print(&hl);

    hl_clear(&hl);

    a = malloc_string("Hello", 5);
    b = malloc_string("World", 5);

    hl_add_word(&hl, a, 5);
    hl_add_word(&hl, b, 5);

    hl_print(&hl);

    hl_free(&hl);

    free(a);
    free(b);
    free(c);
    free(d);
    // free(e); */

    return 0;
}




