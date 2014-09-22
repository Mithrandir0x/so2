#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int c;
  FILE *fp;

  fp = fopen("../proves/fgetc-00", "r");
  if (!fp) {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  while (!feof(fp)) {
    c = fgetc(fp);
    printf("Caracter: %c (byte %d)\n", c, c);
  }

  fclose(fp);

  return 0;
}
