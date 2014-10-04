#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_LENGTH 100

typedef void (*F_PTR)(char*);

// ASCII TABLE (PUNCTUATION GRAPHS)
//   (Source: // http://www.cplusplus.com/reference/cctype/)

//  !    "    #    $    %    &    '    (    )    *    +    ,    -    .    /
// 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2E 0x2F

//  :    ;    <    =    >    ?    @
// 0x3A 0x3B 0x3C 0x3D 0x3E 0x3F 0x40

//  [    \    ]    ^    _    `
// 0x5B 0x5C 0x5D 0x5E 0x5F 0x60


#define IS_WRD_CONCATENATOR(x) x == 0x27

int get_words(FILE *stream, F_PTR f, F_PTR g);
void print_valid_word(char* str);
void print_invalid_word(char* str);

/**
 * Program to extract words from a file passed by argument.
 *
 */
int main(int argc, char *argv[])
{
  if ( argc != 2 )
  {
    printf("usage: practica1 fitxer.txt.\n");
    exit(1);
  }

  FILE *fp;

  fp = fopen(argv[1], "r");
  if (!fp)
  {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  get_words(fp, &print_valid_word, &print_invalid_word);

  fclose(fp);

  return 0;
}

/**
 * This function parses a file, trying to find, for each line, any valid words available.
 *
 * @param stream                A FILE stream structure to the file to be parsed.
 * @param valid_word_callback   A function pointer where valid words will be passed, when found.
 * @param invalid_word_callback A function pointer where invalid words will be passed, when found.
 */
int get_words(FILE *stream, F_PTR valid_word_callback, F_PTR invalid_word_callback)
{
  // This array stores a line of text from the file being read
  char str[BUFFER_LENGTH];
  // This array stores the word to be passed as valid or invalid
  char wrd[BUFFER_LENGTH];

  int flag_invalid_word = 0;

  // i: index of the char index from "str"
  // j: index of the NEXT char to add in "wrd"
  int i = 0, j = 0;
  
  while ( fgets(str, BUFFER_LENGTH, stream) != NULL )
  {
    i = 0;
    while ( str[i] != 0 )
    {
      if ( !flag_invalid_word && ( isalpha(str[i]) || IS_WRD_CONCATENATOR(str[i]) ) ) {
        // Save any valid character in the word buffer
        wrd[j] = tolower(str[i]);
        j++;
      }
      
      else if ( ispunct(str[i]) || isspace(str[i]) ) {
        if ( j > 0 )
        {
          if ( flag_invalid_word )
          {
            // To avoid having to erase all the word buffer, 
            // simply mark the last byte read as \0.
            if ( j < BUFFER_LENGTH ) wrd[j] = 0;
            invalid_word_callback(wrd);
          }
          else
          {
            // To avoid having to erase all the word buffer, 
            // simply mark the last byte read as \0.
            if ( j < BUFFER_LENGTH ) wrd[j] = 0;
            valid_word_callback(wrd);
          }
        }
        
        // Reset the word counter, and the invalid word flag.
        j = 0;
        flag_invalid_word = 0;
      }

      else {
        // Not recognised symbol.
        // Flag the word as invalid,
        // but still, save the character
        wrd[j] = tolower(str[i]);
        j++;
        flag_invalid_word = 1;
      }

      i++;
    }
  }

  return 0;
}

/**
 * This function is used to print valid words.
 *
 * @param str The word stored in a char array
 */
void print_valid_word(char* str)
{
  printf("  Paraula vàlida: %s\n", str);
}

/**
 * This function is used to print invalid words.
 *
 * @param str The word stored in a char array
 */
void print_invalid_word(char* str)
{
  printf("Paraula invàlida: %s\n", str);
}
