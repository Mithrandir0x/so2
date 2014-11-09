#ifndef __STATS_H__
#define __STATS_H__

#include "red-black-tree.h"

/**
 * This structure allows us to save statistical information of a
 * Red-Black Tree containing words and their occurrences.
 *
 * As for now, it stores the occurrences of all words for each size available.
 */
typedef struct {
  int length;            // Real size of the occurrences array
  int *occurrences;      // Array that contains word occurrences for each word's size that exists
  int total_occurrences; // The sum of all word's occurrences from a tree.
} DictionaryStatistics;

void st_initialize(DictionaryStatistics *stats);

void st_extract_statistics(RBTree *tree, DictionaryStatistics *stats);

void st_dump_statistics(DictionaryStatistics *stats);

#endif
