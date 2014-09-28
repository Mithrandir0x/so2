#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LENGTH 10

void original_readfile(char *str, FILE *fp);
void corrected_readfile(char *str, FILE *fp);

int main(void)
{
  char str[BUFFER_LENGTH];
  FILE *fp;

  fp = fopen("fitxer.txt", "r");
  if (!fp) {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  original_readfile(str, fp);

  corrected_readfile(str, fp);

  fclose(fp);

  return 0;
}

void corrected_readfile(char *str, FILE *fp) {
  printf("Corrected version that allows to print all the file:\n");
  
  while ( fgets(str, BUFFER_LENGTH, fp) != NULL ) {
    printf("Linia: %s\n", str);
  }
}

void original_readfile(char *str, FILE *fp) {
  printf("Original version:\n");
  
  fgets(str, BUFFER_LENGTH, fp);
  while ( !feof(fp) ) {
    printf("Linia: %s\n", str);
    fgets(str, BUFFER_LENGTH, fp);
  }
}
