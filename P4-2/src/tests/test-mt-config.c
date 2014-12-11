#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../config.h"

int main(int argc, char **argv)
{
  char *path;
  int n_threads = 1;
  pthread_t tid;
  FilePathList *list;
  ProgressPtr process_file;
  
  process_file = ^(char *file_path, int file_num, int total_files){
    pthread_t tid;
    tid = pthread_self();
    printf("[%10u] Reading file [%s] with id [%d/%d]\n", (unsigned int) tid, file_path, file_num + 1, total_files);
  };

  if ( argc != 3 )
  {
    printf("usage: ./test-mt-config llista_prova.cfg 4\n");
    exit(1);
  }

  path = argv[1];
  n_threads = atoi(argv[2]);

  list = malloc(sizeof(FilePathList));
  
  cfg_init(list);
  cfg_import_config(path, list);

  if ( list->size == 0 )
  {
    cfg_free(list);
    free(list);
    printf("No s'ha pogut carregar la llista de configuració.\n");
    exit(1);
  }

  tid = pthread_self();
  printf("Fil principal: [%10u]\n", (unsigned int) tid);

  cfg_mt_iterate(list, process_file, n_threads);
  
  cfg_free(list);
  free(list);

  return 0;
}
