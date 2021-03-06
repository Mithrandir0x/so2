#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "config.h"

void printFilePathItem(FilePathItem *item)
{
  //printf("{ id: [%d] size: [%d] path: [%s] }\n", item->id, item->size, item->path);
  printf("{ [%p] ", item);

  printf("size: [%d] ", item->size);
  printf("id: [%d] ", item->id);
  printf("path: [%s] ", item->path);

  printf("}\n");
}

void cfg_print(FilePathList *list)
{
  int i;
  FilePathItem *item;

  printf("{ [%p]\n", list);
  
  for ( i = 0 ; i < list->size ; i++ )
  {
    item = &(list->files[i]);

    printFilePathItem(item);
  }

  printf("}\n");
}

void cfg_init(FilePathList *cnt)
{
  cnt->size = 0;
  cnt->sumFilesSize = 0;
  cnt->files = NULL;
  cnt->_nextItemIndex = 0;
}

void cfg_init_pr(FilePathList *cnt, int size)
{
  int i;

  cnt->size = size;
  cnt->sumFilesSize = 0;
  cnt->_nextItemIndex = 0;
  cnt->files = malloc(sizeof(FilePathItem) * cnt->size);

  for ( i = 0 ; i < size ; i++ )
  {
    cnt->files[i].id = -1;
    cnt->files[i].size = 0;
    cnt->files[i].path = NULL;
  }
}

void cfg_insert_file_path(FilePathList *cnt, int id, int size, char *path)
{
  int i = cnt->_nextItemIndex;
  cnt->files[i].id = id;
  cnt->files[i].size = size;
  cnt->files[i].path = malloc((strlen(path) + 1 ) * sizeof(char));
  strcpy(cnt->files[i].path, path);

  cnt->_nextItemIndex++;
}

void cfg_free(FilePathList *cnt)
{
  int i;
  FilePathItem *item;
  
  for ( i = 0 ; i < cnt->size ; i++ )
  {
    item = &(cnt->files[i]);

    free(item->path);
  }
  free(cnt->files);
  free(cnt);
}

void cfg_import_config(char *config_path, FilePathList *cnt)
{
  FILE *config;
  FilePathItem *item;
  struct stat st;
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
  cnt->files = malloc(sizeof(FilePathItem) * cnt->size);
  
  while ( fgets(str, 200, config) != NULL )
  {
    // Do not include newline character when returning the string with the file path
    l = strlen(str);
    if ( str[l - 1] == '\n' ) str[l - 1] = '\0';

    item = &(cnt->files[i]);

    //printf("%d %s %d\n", i, str, l);

    item->id = i;

    stat(str, &st);
    item->size = st.st_size;
    cnt->sumFilesSize += item->size;
    
    item->path = malloc(sizeof(char) * ( l + 1 ));
    strcpy(item->path, str);

    // printFilePathItem(item);

    i++;
  }

  fclose(config);
}

void cfg_iterate(FilePathList *cnt, ProgressPtr callback)
{
  int i = 0;
  
  for ( i = 0 ; i < cnt->size ; i++ )
  {
    if ( cnt->files[i].path != NULL ) callback(cnt->files + i, cnt->size);
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

/* static int cfg_iterator_next(FilePathListIterator *iter, FilePathIteratorItem *item)
{
  if ( iter->index < iter->list->size )
    iter->index++;
  else
    iter->index = -1;

  return iter->index;
} */

/**
 * Thread safe operation to iterate a FilePathList structure
 *
 * @param iter The iterator structure that stores the state of the iteration
 * @param item The returning item from the iterator
 */
static FilePathItem *cfg_mt_iterator_next(FilePathListIterator *iter)
{
  FilePathItem *item = NULL;

  pthread_mutex_lock(&mt_iterator_next_mutex);
  
  if ( ( iter->index + 1 ) < iter->list->size )
  {
    iter->index++;
    item = iter->list->files + iter->index;
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
  
  pthread_t tid;
  
  int parsed_files = 0;
  
  struct timespec start, end;
  FileProcessorStatistics *stats;
  
  FilePathListIterator *iter = params->iter;
  FilePathItem *item;
  ProgressPtr callback = params->callback;
  FilePathList *list = iter->list;
  
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  tid = pthread_self();
  stats = malloc(sizeof(FileProcessorStatistics));

  while ( ( item = cfg_mt_iterator_next(iter) ) != NULL )
  {
    callback(item, list->size);
    parsed_files++;
  }

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
  
  pthread_t tid;
  pthread_t tids[n_threads];
  
  double time_taken;
  struct timespec start, end;
  
  FilePathListIterator *iter;
  FileProcessorParameters *params;
  FileProcessorStatistics **stats;
  
  clock_gettime(CLOCK_MONOTONIC, &start);

  tid = pthread_self();
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

  clock_gettime(CLOCK_MONOTONIC, &end);
  time_taken = end.tv_sec - start.tv_sec;
  time_taken += ( end.tv_nsec - start.tv_nsec) / 1000000000.0;
  printf("Main Thread ID: [%10u] Parsed files: [%3d] Time taken: [%lf] s\n",
    (unsigned int) tid,
    list->size,
    time_taken);
}

/**
 * SCHEDULED MULTI-THREADED SUPPORT FOR ITERATING THE STRUCTURE

 max file size per batch
 max threads
 initial load
 drecreasing load
 total size
 */

static void *th_sch(void *arg)
{
  FileProcessorParameters *params = (FileProcessorParameters *) arg;
  
  pthread_t tid;
  
  struct timespec start, end;
  FileProcessorStatistics *stats;
  
  FilePathListIterator *iter = params->iter;
  ProgressPtr callback = params->callback;
  FilePathList *list = iter->list;
  
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  tid = pthread_self();
  stats = malloc(sizeof(FileProcessorStatistics));

  cfg_iterate(list, callback);

  clock_gettime(CLOCK_MONOTONIC, &end);

  printf("Thread ID [%10u] Total files [%3d]\n", (unsigned int) tid, list->_nextItemIndex);

  stats->thread_id = (unsigned int) tid;
  stats->parsed_files = list->_nextItemIndex;
  stats->time_taken = end.tv_sec - start.tv_sec;
  stats->time_taken += ( end.tv_nsec - start.tv_nsec) / 1000000000.0;

  return((void *) stats);
}

/**
 * Multi-threaded and scheduled version of file iteration.
 * 
 * This function allows to iterate a file list in a multi-threaded way, and allows to
 * balance the load between the threads that will be spawned thanks to a scheduler passed
 * by parameter.
 *
 * The scheduler has to distribute the files throughout new lists of files that each thread
 * will process.
 */
void cfg_sch_mt_iterate(FilePathList *list, ProgressPtr callback, FilePathScheduler scheduler, int n_threads)
{
  int i;
  
  pthread_t tid;
  pthread_t tids[n_threads];
  
  struct timespec start, end;
  double time_taken;
  
  FilePathList **tasks;
  FileProcessorParameters **params;
  FilePathListIterator **iters;
  FileProcessorStatistics **stats;

  clock_gettime(CLOCK_MONOTONIC, &start);

  tid = pthread_self();
  tasks = malloc(sizeof(FilePathList *) * n_threads);
  params = malloc(sizeof(FileProcessorParameters *) * n_threads);
  iters = malloc(sizeof(FilePathListIterator *) * n_threads);
  stats = malloc(sizeof(FileProcessorStatistics) * n_threads);

  for ( i = 0 ; i < n_threads ; i++ )
  {
    tasks[i] = malloc(sizeof(FilePathList));
    iters[i] = cfg_iterator(tasks[i]);
    
    params[i] = malloc(sizeof(FileProcessorParameters));
    params[i]->iter = iters[i];
    params[i]->callback = callback;
  }

  scheduler(list, tasks, n_threads);

  for ( i = 0 ; i < n_threads ; i++ )
  {
    pthread_create(tids + i, NULL, th_sch, (void *)(params[i]));
  }

  for ( i = 0 ; i < n_threads ; i++ )
  {
    pthread_join(tids[i], (void *)(stats + i));
  }

  for ( i = 0 ; i < n_threads ; i++ )
  {
    print_file_processor_statistics(stats[i]);
    
    free(stats[i]);
    free(params[i]);
    free(iters[i]);
    cfg_free(tasks[i]);
  }
  free(stats);
  free(iters);
  free(params);
  free(tasks);

  clock_gettime(CLOCK_MONOTONIC, &end);

  time_taken = end.tv_sec - start.tv_sec;
  time_taken += ( end.tv_nsec - start.tv_nsec) / 1000000000.0;
  printf("Main Thread ID: [%10u] Parsed files: [%3d] Time taken: [%lf] s\n",
    (unsigned int) tid,
    list->size,
    time_taken);
}
