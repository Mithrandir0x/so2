#ifndef __CONFIG__
#define __CONFIG__

typedef struct {
    int id;
    int size;
    char *path;
} FilePathItem;

typedef struct {
  int size;
  int _nextItemIndex;
  long sumFilesSize;
  FilePathItem *files;
} FilePathList;

typedef struct {
    int index;
    FilePathList *list;
} FilePathListIterator;

typedef void (^ProgressPtr)(FilePathItem *, int);

/**
 * Block function header to allow arbitrary partitions of a list of files to be parsed.
 *
 * The idea is to distribute all the files inside the list into "n_threads" new lists
 * so that each thread that will be using that list will parse all those files.
 */
typedef void (^FilePathScheduler)(FilePathList *list, FilePathList **tasks, int n_threads);

void cfg_init(FilePathList *pathContainer);
void cfg_init_pr(FilePathList *pathContainer, int size);
void cfg_free(FilePathList *pathContainer);
void cfg_print(FilePathList *list);

void cfg_insert_file_path(FilePathList *cnt, int id, int size, char *path);

void cfg_import_config(char *config_path, FilePathList *cnt);

void cfg_iterate(FilePathList *cnt, ProgressPtr callback);

void cfg_mt_iterate(FilePathList *list, ProgressPtr callback, int n_threads);
void cfg_sch_mt_iterate(FilePathList *list, ProgressPtr callback, FilePathScheduler scheduler, int n_threads);

#endif

