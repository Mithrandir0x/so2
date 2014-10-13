#ifndef __WORD_UTILS__
#define __WORD_UTILS__

#define WU_BUFFER_LENGTH 100

typedef void (^FunctionWordSizePtr)(char*, int);

int wu_get_words(FILE *stream, FunctionWordSizePtr valid_word_callback, FunctionWordSizePtr invalid_word_callback);

#endif
