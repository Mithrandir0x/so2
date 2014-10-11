#include <stdlib.h>

#include "hash-list.h"

int hash(char *str, int str_len, int max)
{
  int i, hash, sum = 0;
  int seed = HASH_SEED;
  
  for(i = 0; i < str_len; i++)
    sum = sum * seed + (int) str[i];

  hash = sum % max;
  
  return hash;
}

void hl_initialize(HashList *hl, int bucket_size)
{
  int i = bucket_size;
  List *l;
  
  hl->size = bucket_size;
  hl->buckets = malloc(sizeof(List) * bucket_size);
  
  while ( i > 0 )
  {
    l = &(hl->buckets[i]);
    initList(l);
    i--;
  }
}

void hl_add_word(HashList *hl, char *str, int str_len)
{
  int h;
  List *l;
  ListData *ld;
  
  h = hash(str, str_len, hl->size);
  
  l = &(hl->buckets[h]);
  
  // TODO Fix type definition required by linked list
  //ld = findList(l, str);
  if ( ld != NULL )
  {
    ld->numTimes++;
  }
  else
  {
    ld = malloc(sizeof(ListData));
    // TODO Fix type definition required by linked list
    //ld->primary_key = str;
    ld->numTimes = 1;
    
    insertList(l, ld);
  }
}

void hl_free(HashList *hl)
{
  int i = hl->size;
  List *l;
  
  while ( i > 0 )
  {
    l = &(hl->buckets[i]);
    
    deleteList(l);
    free(&(hl->buckets[i]));
    
    i--;
  }
  
  hl->size = 0;
  free(hl->buckets);
}
