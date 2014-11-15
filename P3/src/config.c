#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

void cfg_init(FilePathList *cnt)
{
  cnt->size = 0;
  cnt->files = NULL;
}

void cfg_free(FilePathList *cnt)
{
  int i;
  
  for ( i = 0 ; i < cnt->size ; i++ )
  {
    free(cnt->files[i]);
  }
  free(cnt->files);
}

void cfg_get_file_list(char *config_path, int buffer_size, ProgressPtr callback)
{
  FILE *config;
  char str[buffer_size];
  char *r;
  int total_files = 0;
  int read = 0; 
  int ln;
  
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
    // Do not include newline character when returning the string with the file path
    ln = strlen(str) - 1;

    if ( str[ln] == '\n' ) str[ln] = '\0';
    
    callback(str, read, total_files);
    read++;
  }

  fclose(config);
}

void cfg_import_config(char *config_path, FilePathList *cnt)
{
  FILE *config;
  char str[200];
  char *r;
  int i = 0;
  int l;
  
  config = fopen(config_path, "r");
  
  if (config == 0)
  {
    printf("El fitxer [%s] no existeix o no es pot obrir.\n", config_path);
    return;
  }
  
  r = fgets(str, 200, config);
  if (r == NULL)
  {  
    printf("El fitxer [%s] esta buit.\n", config_path);
    return;
  }
  cnt->size = atoi(str);
  
  cnt->files = malloc(sizeof(char *) * cnt->size);
  while ( fgets(str, 200, config) != NULL )
  {
    // Do not include newline character when returning the string with the file path
    l = strlen(str);
    if ( str[l - 1] == '\n' ) str[l - 1] = '\0';

    //printf("%d %s %d\n", i, str, l);
    
    cnt->files[i] = malloc(sizeof(char) * ( l + 1 ));
    strcpy(cnt->files[i], str);

    i++;
  }

  fclose(config);
}

void cfg_iterate(FilePathList *cnt, ProgressPtr callback)
{
  int i = 0;
  
  for ( i = 0 ; i < cnt->size ; i++ )
  {
    callback(cnt->files[i], i, cnt->size);
  }
}
