#ifndef __CONFIG__
#define __CONFIG__

typedef struct {
  int size;
  char **files;
} FilePathList;

typedef struct {
    int index;
    FilePathList *list;
} FilePathListIterator;

typedef void (^ProgressPtr)(char *, int, int);

void cfg_init(FilePathList *pathContainer);
void cfg_free(FilePathList *pathContainer);

void cfg_import_config(char *config_path, FilePathList *cnt);

void cfg_iterate(FilePathList *cnt, ProgressPtr callback);

void cfg_get_file_list(char *config_path, int buffer_size, ProgressPtr callback);

void cfg_mt_iterate(FilePathList *list, ProgressPtr callback, int n_threads);

#endif

