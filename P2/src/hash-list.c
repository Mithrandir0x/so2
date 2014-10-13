#include <stdio.h>
#include <stdlib.h>

#include "hash-list.h"

static int hash(char *str, int str_len, int max)
{
  unsigned int i, hash, sum = 0;
  unsigned int seed = HASH_SEED;
  
  for(i = 0; i < str_len; i++)
    sum = sum * seed + (int) str[i];

  hash = sum % max;
  
  return hash;
}

void hl_initialize(HashList *hl, int bucket_size)
{
  int i;
  List *l;
  
  hl->size = bucket_size;
  hl->buckets = malloc(sizeof(List) * bucket_size);

  for ( i = 0 ; i < bucket_size ; i++ )
  {
    l = &(hl->buckets[i]);
    initList(l);
  }
}

int hl_add_word(HashList *hl, char *str, int str_len)
{
  int h, r;
  List *l;
  ListData *ld;
  
  h = hash(str, str_len, hl->size);

  // printf("[%16s] Calculated hash for [%s]: [%d]\n", "hl_add_word", str, h);
  
  l = &(hl->buckets[h]);
  
  ld = findList(l, str);
  if ( ld != NULL )
  {
    ld->numTimes++;
    r = 1;
  }
  else
  {
    ld = malloc(sizeof(ListData));
    ld->primary_key = str;
    ld->numTimes = 1;
    
    insertList(l, ld);

    r = 0;
  }

  return r;
}

void hl_clear(HashList *hl)
{
  int hl_size = hl->size;
  hl_free(hl);
  hl_initialize(hl, hl_size);
}

void hl_free(HashList *hl)
{
  int i, n;
  List *l;

  for ( i = 0, n = hl->size ; i < n ; i++ )
  {
    l = &(hl->buckets[i]);
    deleteList(l);
  }
  
  hl->size = 0;
  free(hl->buckets);
}

void hl_print(HashList *hl)
{
  int i = 0, n = hl->size;
  List *l;

  printf("[\n");
  for ( ; i < n ; i++ )
  {
    l = &(hl->buckets[i]);
    
    printf("  [%3d] List[%d] {\n", i, l->numItems);
    dumpList(l);
    printf("  }\n");
  }
  printf("]\n");
}
