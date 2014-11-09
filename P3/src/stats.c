#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stats.h"

#define ST_BASE_SIZE_OCCURRENCES 100

void st_initialize(DictionaryStatistics *stats)
{
  stats->length = ST_BASE_SIZE_OCCURRENCES;
  stats->occurrences = calloc(ST_BASE_SIZE_OCCURRENCES, sizeof(int));
  stats->total_occurrences = 0;
}

void st_free(DictionaryStatistics *stats)
{
  free(stats->occurrences);
  stats->length = 0;
  stats->total_occurrences = 0;
}

void st_printf(DictionaryStatistics *stats)
{
  int i;
  
  printf("{\n  length: [%d],\n  total_occurrences: [%d],\n  occurrences: [\n", stats->length, stats->total_occurrences);
  for ( i = 0 ; i < stats->length ; i++ )
  {
    if ( stats->occurrences[i] > 0 )
      printf("    [%d]: [%d]\n", i, stats->occurrences[i]);
  }
  printf("  ]\n}\n");
}

void st_extract_statistics(RBTree *tree, DictionaryStatistics *stats)
{
  IterationPtr putWordInDictionary = ^(RBData *data) {
    int *occurrences;
    int length, diff, old_size, i;

    length = strlen(data->primary_key);

    if ( length >= stats->length )
    {
      old_size = stats->length;
      diff = length - stats->length + 1;
      
      // Create new slots as much as the new size is required
      occurrences = realloc(stats->occurrences, ( diff ) * sizeof(int));
      
      if ( occurrences == NULL )
      {
        printf("Error while trying to reallocate occurrences array\n");
        exit(1);
      }
      else
      {
        stats->occurrences = occurrences;
      }

      // Realloc does not initialize data
      for ( i = old_size ; i < stats->length ; i++ ) stats->occurrences[i] = 0;
    }

    stats->occurrences[length] += data->total_words;
    stats->total_occurrences += data->total_words;
  };
  
  iterateTree(tree, putWordInDictionary);
}

void st_dump_statistics(DictionaryStatistics *stats, char *path)
{
  FILE *data;
  FILE *gnuplot;
  int i;
  float f;
  char cmd[100];
  
  int gnuplot_cmd_size = 7;
  char *gnuplot_cmds[] = {
    /* 01 */ "reset",
    /* 02 */ "set terminal png size 1920,720",
    /* 03 */ "set boxwidth 0.5",
    /* 04 */ "set style fill solid 1.00 noborder",
    /* 05 */ "set style line 1 lc rgb \"blue\"",
    /* 06 */ "set xlabel \"Word Sizes\"",
    /* 07 */ "set ylabel \"Probabilities (%)\""
  };

  // Dump data to temporary file
  data = fopen("stats_dump.dat", "w+");
  if ( data == 0 )
  {
    printf("Cannot create temporary file to dump statistics data\n");
    exit(1);
  }

  for ( i = 0 ; i < stats->length ; i++ )
  {
    if ( stats->occurrences[i] > 0 )
    {
      f = ( (float) stats->occurrences[i] / (float) stats->total_occurrences ) * 100.0;
      fprintf(data, "%d %f\n", i, f);
    }
  }
  fclose(data);

  strcpy(cmd, "gnuplot >");
  strcat(cmd, path);

  // Pipe gnuplot commands and plot the damn thing
  gnuplot = popen(cmd, "w");
  if ( gnuplot == 0 )
  {
    printf("Cannot create gnuplot pipe to send plotting instructions\n");
    exit(1);
  }

  for ( i = 0 ; i < gnuplot_cmd_size ; i++ )
  {
    strcpy(cmd, gnuplot_cmds[i]);
    strcat(cmd, "\n");
    printf("%s", cmd);
    fputs(cmd, gnuplot);
  }
  fputs("plot \"stats_dump.dat\" with boxes ls 1\n", gnuplot);

  if ( pclose(gnuplot) == 1 )
  {
    printf("Cannot close gnuplot pipe\n");
    exit(1);
  }
}
