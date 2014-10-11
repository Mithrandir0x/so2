
typedef struct file_list_t
{
  int size;
  char **file_paths;
} file_list;

/**
 * Return a list of files to be read given a configuration file.
 * 
 * A configuration file should start with the number of files to be
 * read at the first line.
 * 
 * The next lines should be all the paths to the files to be read.
 * 
 * @param config_path The config file to read from
 */
file_list_t get_file_list(char *config_path)
{
  
}