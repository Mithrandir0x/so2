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

void st_dump_statistics(DictionaryStatistics *stats)
{
}
