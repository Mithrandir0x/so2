CC     = gcc
CFLAGS = -Wall -Werror -g
LFLAGS = -lm

SOURCE_C = $(wildcard *.c)
SOURCE_O = $(patsubst %.c,%.o,$(SOURCE_C))
SOURCE   = $(patsubst %.c,%.exe,$(SOURCE_C))

$(SOURCE): $(SOURCE_O)
	$(CC) $@.o -o $@.exe $(LFLAGS)

$(SOURCE_O): $(SOURCE_C)
	$(CC) $(CFLAGS) -c $^

all: $(SOURCE)

clean:
	rm -f $(SOURCE_O)
	rm -f $(SOURCE) 
