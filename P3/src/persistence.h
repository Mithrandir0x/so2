
#ifndef __PERSISTENCE_H__
#define __PERSISTENCE_H__

#include "red-black-tree.h"

int prs_save_global_structure(RBTree *tree, char *path);
RBTree* prs_load_global_structure(char *path);

#endif
