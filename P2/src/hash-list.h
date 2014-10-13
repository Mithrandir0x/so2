
#ifndef __HASH_LIST__
#define __HASH_LIST__

#include "linked-list.h"

#define HASH_SEED        131

typedef struct hash_list_t {
  int   size;
  List *buckets;
} HashList;

void hl_initialize(HashList *hashlist, int bucket_size);
void hl_add_word(HashList *hl, char *str, int str_len);
void hl_free(HashList *hl);
void hl_print(HashList *hl);

#endif
