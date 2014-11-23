#include <time.h>
#include <pthread.h>
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

/**
 * MULTI-THREADING SUPPORT FOR ITERATING THE STRUCTURE
 */

static pthread_mutex_t mt_iterator_next_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
  FilePathListIterator *iter;
  ProgressPtr callback;
} FileProcessorParameters;

typedef struct {
  int id;
  char *path;
} FilePathIteratorItem;

typedef struct {
  unsigned int thread_id;
  int parsed_files;
  double time_taken;
} FileProcessorStatistics;

static void print_file_processor_statistics(FileProcessorStatistics *stats)
{
  printf("Thread ID: [%10u] Parsed files: [%3d] Time taken: [%lf] s\n",
    stats->thread_id,
    stats->parsed_files,
    stats->time_taken
  );
}

/**
 * Returns a pointer to an iterator structure ready to iterate a FilePathList.
 *
 * @param list The FilePathList to iterate
 * @returns A pointer to an instance created of type FilePathListIterator.
 */
static FilePathListIterator *cfg_iterator(FilePathList *list)
{
  FilePathListIterator *iter;

  iter = malloc(sizeof(FilePathListIterator));

  iter->index = 0;
  iter->list = list;

  return iter;
}

/**
 * Thread safe operation to iterate a FilePathList structure
 *
 * @param iter The iterator structure that stores the state of the iteration
 * @param item The returning item from the iterator
 */
static FilePathIteratorItem *cfg_mt_iterator_next(FilePathListIterator *iter, FilePathIteratorItem *item)
{
  int i, size;
  FilePathList *list;

  pthread_mutex_lock(&mt_iterator_next_mutex);

  item->id = -1;
  item->path = NULL;

  list = iter->list;
  i = iter->index;
  size = list->size;

  if ( i < size )
  {
    item->id = i;
    item->path = list->files[i];
    iter->index++;
  }

  pthread_mutex_unlock(&mt_iterator_next_mutex);

  return item;
}

/**
 * Thread implementation used by cfg_mt_iterate function.
 *
 * Each instanced thread will request a new file to be parsed. The fastest thread
 * will get most of the files.
 */
static void *th_call_progress_callback(void *arg)
{
  FileProcessorParameters *params = (FileProcessorParameters *) arg;
  
  int parsed_files = 0;
  struct timespec start, end;
  FileProcessorStatistics *stats;
  FilePathListIterator *iter = params->iter;
  ProgressPtr callback = params->callback;
  FilePathList *list = iter->list;
  FilePathIteratorItem *item;
  pthread_t tid;
  
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  tid = pthread_self();
  item = malloc(sizeof(FilePathIteratorItem));
  stats = malloc(sizeof(FileProcessorStatistics));

  while ( ( item = cfg_mt_iterator_next(iter, item) )->id != -1 )
  {
    callback(item->path, item->id, list->size);
    parsed_files++;
  }

  free(item);

  clock_gettime(CLOCK_MONOTONIC, &end);

  stats->thread_id = (unsigned int) tid;
  stats->parsed_files = parsed_files;
  stats->time_taken = end.tv_sec - start.tv_sec;
  stats->time_taken += ( end.tv_nsec - start.tv_nsec) / 1000000000.0;

  return((void *) stats);
}

/**
 * Multi-threaded iteration function for FilePathList structures.
 *
 * This is a naive approximation of load balance. The fastest thread will be able
 * to get the most of the files to be parsed. A lock is placed when fetching a new
 * file from the list.
 *
 * @param list      Pointer to the list to iterate
 * @param callback  Function to be called for each file in the list
 * @param n_threads Number of threads to spawn that will carry all the file processing
 */
void cfg_mt_iterate(FilePathList *list, ProgressPtr callback, int n_threads)
{
  int i;  
  pthread_t tids[n_threads];
  FilePathListIterator *iter;
  FileProcessorParameters *params;
  FileProcessorStatistics **stats;

  iter = cfg_iterator(list);
  params = malloc(sizeof(FileProcessorParameters));
  stats = malloc(sizeof(FileProcessorStatistics) * n_threads);

  params->iter = iter;
  params->callback = callback;

  for ( i = 0 ; i < n_threads ; i++ )
  {
    pthread_create(tids + i, NULL, th_call_progress_callback, (void *) params);
  }

  for ( i = 0 ; i < n_threads ; i++ )
  {
    pthread_join(tids[i], (void *)(stats + i));
  }

  free(iter);
  free(params);

  for ( i = 0 ; i < n_threads ; i++ )
  {
    print_file_processor_statistics(stats[i]);
    free(stats[i]);
  }

  free(stats);
}
