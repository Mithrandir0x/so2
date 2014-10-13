#include <stdio.h>
#include <stdlib.h>

#include "../hash-list.h"

int main(int argc, char **argv)
{
    HashList hl;

    hl_initialize(&hl, 3);

    hl_add_word(&hl, "Hello", 5);
    hl_add_word(&hl, "Hello", 5);
    
    hl_add_word(&hl, "World", 5);
    hl_add_word(&hl, "World", 5);
    
    hl_add_word(&hl, "Hello World", 11);

    hl_print(&hl);

    hl_free(&hl);

    return 0;
}
