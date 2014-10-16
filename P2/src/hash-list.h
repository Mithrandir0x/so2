
#ifndef __HASH_LIST__
#define __HASH_LIST__

#include "linked-list.h"

#define HASH_SEED        131

typedef struct hash_list_t {
  int   size;
  List *buckets;
} HashList;

typedef struct hash_list_iteratort_ {
  int index;
  HashList *hashList;
} HashListIterator;

/* Constructors and destructors */
void hl_initialize(HashList *hashlist, int bucket_size);
void hl_clear(HashList *hl);
void hl_free(HashList *hl);

/* Data Manipulation */
int hl_add_word(HashList *hl, char *str, int str_len);

/* Miscellaneous */
void hl_print(HashList *hl);

/* Iterators */
HashListIterator* hl_iterator(HashList *hl);
List *hl_next(HashListIterator *hli);

#endif
