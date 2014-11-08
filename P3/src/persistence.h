
#ifndef __PERSISTENCE_H__
#define __PERSISTENCE_H__

#include "red-black-tree.h"

/**
 * 
 */
#define PRS_FILE_NOT_FOUND 1
#define PRS_BAD_FORMAT     2

int prs_save(RBTree *tree, char *path);
int prs_load(RBTree *tree, char *path);

#endif
