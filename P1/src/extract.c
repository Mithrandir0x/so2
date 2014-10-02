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

void print_word(char* str);
int read_text_file(FILE *stream, F_PTR f, F_PTR g);
void print_valid_word(char* str);
void print_invalid_word(char* str);

int main()
{
  FILE *fp;

  fp = fopen("fitxer.txt", "r");
  if (!fp) {
    printf("No he pogut obrir fitxer.\n");
    exit(1);
  }

  read_text_file(fp, &print_valid_word, &print_invalid_word);

  fclose(fp);

  return 0;
}

int read_text_file(FILE *stream, F_PTR valid_word_callback, F_PTR invalid_word_callback)
{
  char str[BUFFER_LENGTH];
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
            if ( j < BUFFER_LENGTH ) wrd[j] = 0;
            invalid_word_callback(wrd);
          }
          else
          {
            // Per evitar tenir que esborrar el buffer de paraula,
            // marquem el byte llegit com a NUL.
            if ( j < BUFFER_LENGTH ) wrd[j] = 0;
            valid_word_callback(wrd);
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

void print_valid_word(char* str)
{
  printf("  Paraula vàlida: %s\n", str);
}

void print_invalid_word(char* str)
{
  printf("Paraula invàlida: %s\n", str);
}
