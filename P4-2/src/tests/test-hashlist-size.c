#include <stdio.h>
#include <stdlib.h>

#include "../hash-list.h"
#include "../word-utils.h"

void create_local_structure(HashList *hl, char *path)
{
    FILE *text;
    FunctionWordSizePtr saveWord;

    saveWord = ^(char *str, int length) {
        int r;
        r = hl_add_word(hl, str, length);

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

int main(int argc, char **argv)
{
  HashList hl;
  int bucket_size;

  if ( argc != 3 )
  {
    printf("usage: test-hashlist-size fitxer.txt 10\n");
    exit(1);
  }

  bucket_size = atoi(argv[2]);

  hl_initialize(&hl, bucket_size);
  create_local_structure(&hl, argv[1]);
  hl_print_stats(&hl);
  hl_free(&hl);

  return 0;
}
