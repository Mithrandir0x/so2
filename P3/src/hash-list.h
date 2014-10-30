
#ifndef __HASH_LIST__
#define __HASH_LIST__

#include "linked-list.h"

#define HASH_SEED        131

/**
 * This structure allows to store a custom hash map structure. It stores a fixed size
 * amount of buckets of linked lists of strings.
 *
 * Each entry of the map has an integer (acting as key), associated to a linked list 
 * (acting as value). The value of such integer is the hash value shared among all
 * the strings stored inside the linked list.
 */
typedef struct hash_list_t {
  int   size;
  List *buckets;
} HashList;

/**
 * A utility structure to store an iterator's state.
 */
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
void hl_print_stats(HashList *hl);

/* Iterators */
HashListIterator* hl_iterator(HashList *hl);
List *hl_next(HashListIterator *hli);

#endif
