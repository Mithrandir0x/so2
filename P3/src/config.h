#ifndef __CONFIG__
#define __CONFIG__

typedef void (^ProgressPtr)(char *, int, int);

void cfg_get_file_list(char *config_path, int buffer_size, ProgressPtr callback);

#endif
