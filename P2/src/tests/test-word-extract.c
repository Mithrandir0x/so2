#include <stdio.h>
#include <stdlib.h>

#include "../word-utils.h"

int main(int argc, char *argv[])
{
  FILE *fp;
  FunctionWordSizePtr print_valid_word;
  FunctionWordSizePtr print_invalid_word;

  print_valid_word = ^(char* str, int length) {
    printf("  Paraula vàlida: %s [%d]\n", str, length);
  };

  print_invalid_word = ^(char* str, int length) {
    printf("Paraula invàlida: %s [%d]\n", str, length);
  };
  
  if ( argc != 2 )
  {
    printf("usage: test-word-extract fitxer.txt\n");
    exit(1);
  }


  fp = fopen(argv[1], "r");
  if (!fp)
  {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  wu_get_words(fp, print_valid_word, print_invalid_word);

  fclose(fp);

  return 0;
}
