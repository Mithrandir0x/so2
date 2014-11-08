#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "persistence.h"

static int magic_number_a = 0x4342535F;
static int magic_number_b = 0x534C4F5F;

// Well, I hope nobody ever tries to create a tree with more than 1000M files...
static int stop_number    = 0x50544F53;

/**
 * Save a Red-Black Tree structure in the path passed by parameter.
 *
 * @param tree Pointer to the Red-Black Tree structure that saves all the word counts for each file
 * @param path 
 */
int prs_save(RBTree *tree, char *path)
{
  __block FILE *file;
  __block int amount_files;

  IterationPtr dumpNodeData = ^(RBData *data) {
    int i;
    int key_size;

    key_size = strlen(data->primary_key);
    
    fwrite(&key_size, sizeof(int), 1, file);
    fwrite(data->primary_key, sizeof(char), key_size, file);
    fwrite(&data->total, sizeof(int), 1, file);
    fwrite(&data->total_words, sizeof(int), 1, file);

    // printf("\n[[%d], [%s], [%d], [%d]]\n", key_size, data->primary_key, data->total, data->total_words);
    
    // Only save those file indexes where the word appears
    for ( i = 0 ; i < amount_files ; i++ )
    {
      if ( data->tpf[i] > 0 )
      {
        // printf("[%s]: [%d, %d]\n", data->primary_key, i, data->tpf[i]);
        
        fwrite(&i, sizeof(int), 1, file);
        fwrite(data->tpf + i, sizeof(int), 1, file);
      }
    }

    // To know when to stop reading pair of values, we add a 
    fwrite(&stop_number, sizeof(int), 1, file);
  };

  file = fopen(path, "w+");
  
  if ( file == 0 )
  {
    printf("El fitxer [%s] no existeix o no es pot obrir.\n", path);
    return PRS_FILE_NOT_FOUND;
  }

  // Begin the ritual
  fwrite(&magic_number_a, sizeof(int), 1, file);
  fwrite(&magic_number_b, sizeof(int), 1, file);
  fwrite(&tree->num_files, sizeof(int), 1, file);
  fwrite(&tree->num_words, sizeof(int), 1, file);

  // Offer the tree nodes to the vessel
  amount_files = tree->num_files;
  iterateTree(tree, dumpNodeData);
  
  // Seal the vessel with permission of the all mighty Kernel
  fclose(file);

  //printf("Total Files: [%d]\n", tree->num_files);
  //printf("Total Words: [%d]\n", tree->num_words);

  return 0;
}

int prs_load(RBTree *tree, char *path)
{
  FILE *file;
  
  int i;
  int mn_a, mn_b;
  int aux, aux2;
  int num_words;

  RBData *data;

  file = fopen(path, "r");
  
  if ( file == 0 )
  {
    printf("El fitxer [%s] no existeix o no es pot obrir.\n", path);
    return PRS_FILE_NOT_FOUND;
  }

  // Begin the summoning
  fread(&mn_a, sizeof(int), 1, file);
  fread(&mn_b, sizeof(int), 1, file);
  if ( mn_a != magic_number_a && mn_b != magic_number_b )
  {
    printf("El fitxer [%s] té un format no compatible.\n", path);
    free(tree);
    return PRS_BAD_FORMAT;
  }

  fread(&aux, sizeof(int), 1, file);
  fread(&num_words, sizeof(int), 1, file);
  
  initTree(tree, aux);

  //printf("Total Files: [%d]\n", tree->num_files);
  //printf("Total Words: [%d]\n", tree->num_words);

  // Parse each word entry and add it to the tree
  for ( i = 0 ; i < num_words ; i++ )
  {
    // printf("\n%d/%d", i, tree->num_words);

    data = malloc(sizeof(RBData));
    
    fread(&aux, sizeof(int), 1, file);
    
    data->primary_key = calloc(aux + 1, sizeof(char));
    data->tpf = calloc(tree->num_files, sizeof(int));
    
    fread(data->primary_key, sizeof(char), aux, file);
    fread(&data->total, sizeof(int), 1, file);
    fread(&data->total_words, sizeof(int), 1, file);

    // printf("[[%d], [%s], [%d], [%d]]\n", aux, data->primary_key, data->total, data->total_words);

    fread(&aux, sizeof(int), 1, file);
    while ( aux != stop_number )
    {
      fread(&aux2, sizeof(int), 1, file);
      
      //printf("[%s]: [%d, %d]\n", data->primary_key, aux, aux2);
      
      data->tpf[aux] = aux2;

      fread(&aux, sizeof(int), 1, file);
    }

    insertNode(tree, data);
  }

  // Give thanks to the Kernel
  fclose(file);
  
  return 0;
}
