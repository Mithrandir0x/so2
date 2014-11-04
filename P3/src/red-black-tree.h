/**
 *
 * Red-black tree header 
 *
 * See red-black-tree.h for details.
 * 
 *
 */
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

#define TYPE_RBTREE_PRIMARY_KEY char*

/**
 *
 * This structure holds the information to be stored at each node. Change this
 * structure according to your needs.  In order to make this library work, you
 * also need to adapt the functions compLT, compEQ and freeRBData. For the
 * current implementation the "primary_key" member is used to index data within the
 * tree. 
 *
 */

typedef struct RBData_
{
  // The variable used to index the tree has to be called "primary_key".
  // The type may be any you want (float, char *, etc)
  TYPE_RBTREE_PRIMARY_KEY primary_key;     

  // This is the additional information that will be stored
  // within the structure.
  int *tpf;   // Times Per File. How many times a word has appeared in a file?
  int total;  // Number of files. How many files does the word appear in?
  int num_files; // Number of files. Utility variable to iterate "tpf".
  int total_words; // Total amount of times that a word appears for all files being parsed.
} RBData;

/**
 *
 * The node structure 
 *
 */

typedef enum { BLACK, RED } nodeColor;

typedef struct Node_ {
    /* For internal use of the structure. Do not change. */
    struct Node_ *left;         /* left child */
    struct Node_ *right;        /* right child */
    struct Node_ *parent;       /* parent */
    nodeColor color;            /* node color (BLACK, RED) */

    /* Data to be stored at each node */
    RBData *data;                    /* data stored in node */
} Node;

/**
 *
 * The tree structure. It just contains the root node, from
 * which we may go through all the nodes of the binary tree.
 *
 */

typedef struct RBTree_ {
  Node *root;                   /* root of Red-Black tree */
  int num_files;                /* amount of files to be parsed */
} RBTree;

typedef void (^IterationPtr)(RBData *);

/*
 * Function headers. Note that not all the functions of
 * red-black-tree.c have been included here.
 */

void initTree(RBTree *tree, int num_files);
void insertNode(RBTree *tree, RBData *data);
RBData *findNode(RBTree *tree, TYPE_RBTREE_PRIMARY_KEY primary_key); 
void deleteTree(RBTree *tree);
void printTree(RBTree *tree);

void iterateTree(RBTree *tree, IterationPtr callback);

#endif
