#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "word-utils.h"

// ASCII TABLE (PUNCTUATION GRAPHS)
//   (Source: // http://www.cplusplus.com/reference/cctype/)

//  !    "    #    $    %    &    '    (    )    *    +    ,    -    .    /
// 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2E 0x2F

//  :    ;    <    =    >    ?    @
// 0x3A 0x3B 0x3C 0x3D 0x3E 0x3F 0x40

//  [    \    ]    ^    _    `
// 0x5B 0x5C 0x5D 0x5E 0x5F 0x60

#define IS_WRD_CONCATENATOR(x) x == 0x27

int wu_get_words(FILE *stream, FunctionWordSizePtr valid_word_callback, FunctionWordSizePtr invalid_word_callback)
{
  char str[WU_BUFFER_LENGTH];
  char wrd[WU_BUFFER_LENGTH];
  char *copybuffer;

  int flag_invalid_word = 0;

  // i: index of the char index from "str"
  // j: index of the NEXT char to add in "wrd"
  int i = 0, j = 0;
  
  while ( fgets(str, WU_BUFFER_LENGTH, stream) != NULL )
  {
    i = 0;
    while ( str[i] != 0 )
    {
      if ( !flag_invalid_word && ( isalpha(str[i]) || IS_WRD_CONCATENATOR(str[i]) ) ) {
        wrd[j] = tolower(str[i]);
        j++;
      }
      
      else if ( ispunct(str[i]) || isspace(str[i]) ) {
        if ( j > 0 )
        {
          if ( flag_invalid_word )
          {
            // Per evitar tenir que esborrar el buffer de paraula,
            // marquem el byte llegit com a NUL.
            if ( j < WU_BUFFER_LENGTH ) wrd[j] = 0;
            
            if ( invalid_word_callback != NULL ) invalid_word_callback(wrd, j);
          }
          else
          {
            // Per evitar tenir que esborrar el buffer de paraula,
            // marquem el byte llegit com a NUL.
            if ( j < WU_BUFFER_LENGTH ) wrd[j] = 0;
            
            // Quan s'ha trobat una paraula vàlida, en comptes de donar el buffer
            // on es guarda la paraula, fem una còpia de la paraula al HEAP.
            //
            // Queda a discreció de la persona que implementa el callback d'alliberar
            // la memòria reservada en aquest punt.
            copybuffer = malloc(j * sizeof(char) + ( 1 * sizeof(char) ));
            strcpy(copybuffer, wrd);
            
            valid_word_callback(copybuffer, j);
          }
        }
        j = 0;
        flag_invalid_word = 0;
      }

      else {
        // Not recognised symbol
        wrd[j] = tolower(str[i]);
        j++;
        flag_invalid_word = 1;
      }

      i++;
    }
  }

  return 0;
}
