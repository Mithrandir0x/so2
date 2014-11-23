#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash-list.h"

/**
 * Internal string hashing function
 *
 * @param str     String to be hashed
 * @param str_len String size
 * @param max     Maximum value allowed for the hash value returned
 */
static int hash(char *str, int str_len, int max)
{
  unsigned int i, hash, sum = 0;
  unsigned int seed = HASH_SEED;
  
  for(i = 0; i < str_len; i++)
    sum = sum * seed + (int) str[i];

  hash = sum % max;
  
  return hash;
}

/**
 * Initialize a HashList structure with a fixed-size capacity.
 *
 * @param hl          A HashList pointer to be initialized
 * @param bucket_size Indicate the capacity of the hash map
 */
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

/**
 * Stores a word in the hash-list.
 *
 * @param hl      A HashList structure
 * @param str     The string to be saved
 * @param str_len Length of the string
 * @returns Whether the word has been stored, or simply incremented the word counter
 */
int hl_add_word(HashList *hl, char *str, int str_len)
{
  int h, r;
  char *copybuffer;
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
    copybuffer = malloc((strlen(str) + 1 ) * sizeof(char));
    strcpy(copybuffer, str);
    
    ld = malloc(sizeof(ListData));
    ld->primary_key = copybuffer;
    ld->numTimes = 1;
    
    insertList(l, ld);

    r = 0;
  }

  return r;
}

/**
 * Empty the hash-list and initialize it again.
 *
 * @param hl A hash-list structure
 */
void hl_clear(HashList *hl)
{
  int hl_size = hl->size;
  hl_free(hl);
  hl_initialize(hl, hl_size);
}

/**
 * Frees the structure.
 *
 * @param hl A hash-list structure
 */
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

/**
 * Prints in terminal a human readable representation of the hash-list.
 *
 * @param hl A hash-list structure
 */
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

/**
 * Prints in terminal a gnuplot representation of the hash-list.
 *
 * @param hl A hash-list structure
 */
void hl_print_stats(HashList *hl)
{
  int i = 0, n = hl->size, k = 0;
  List *l;

  for ( ; i < n ; i++, k = 0 )
  {
    l = &(hl->buckets[i]);
    printf("%d %d %d\n", i, i, l->numItems);
  }
}

/**
 * Creates an iterator ready to loop the hash-list.
 *
 * @param hl A hash-list structure
 */
HashListIterator* hl_iterator(HashList *hl)
{
  HashListIterator *iter;

  iter = malloc(sizeof(HashListIterator));

  iter->index = 0;
  iter->hashList = hl;

  return iter;
}

/**
 * Returns the next list available in the iterator.
 *
 * @param iter The iterator to iterate
 */
List* hl_next(HashListIterator *iter)
{
  int i, size;
  HashList *hashList;
  List *list;

  hashList = iter->hashList;

  i = iter->index;
  size = hashList->size;
  
  if ( i < size )
  {
    list = &(hashList->buckets[i]);
    iter->index++;
    return list;
  }

  return NULL;
}

/**
 * A utility method to iterate a hash-list structure using blocks.
 *
 * @param hl       A hash-list structure
 * @param callback A callback to be invoked when iterating the hash-list structure
 */
void hl_iterate(HashList *hl, OnHashListIteration callback)
{
  HashListIterator *iter;
  List *l;

  iter = hl_iterator(hl);

  while ( ( l = hl_next(iter) ) != NULL )
  {
    callback(l, iter->index);
  }
}
