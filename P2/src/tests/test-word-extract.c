#include <stdio.h>
#include <stdlib.h>

#include "../word-utils.h"

void print_valid_word(char* str, int length)
{
  printf("  Paraula vàlida: %s [%d]\n", str, length);
}

void print_invalid_word(char* str, int length)
{
  printf("Paraula invàlida: %s [%d]\n", str, length);
}

int main(int argc, char *argv[])
{
  if ( argc != 2 )
  {
    printf("usage: test-word-extract fitxer.txt\n");
    exit(1);
  }

  FILE *fp;

  fp = fopen(argv[1], "r");
  if (!fp)
  {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  wu_get_words(fp, &print_valid_word, &print_invalid_word);

  fclose(fp);

  return 0;
}
