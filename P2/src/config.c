#include <stdio.h>
#include <stdlib.h>

#include "config.h"

void cfg_get_file_list(char *config_path, int buffer_size, ProgressPtr callback)
{
  FILE *config;
  char str[buffer_size];
  char *r;
  int total_files = 0;
  int read = 0;
  
  
  config = fopen(config_path, "r");
  
  if (config == 0)
  {
    printf("El fitxer [%s] no existeix o no es pot obrir.\n", config_path);
    return;
  }
  r = fgets(str, buffer_size, config);
  
  if (r == NULL)
  {  
    printf("El fitxer [%s] esta buit.\n", config_path);
    return;
  }
  total_files = atoi(str);
  
  while ( fgets(str, buffer_size, config) != NULL )
  {
   callback(str, read, total_files);
   read++;
   
  }
  
  fclose(config);
}
