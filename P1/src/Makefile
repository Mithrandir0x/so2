CC     = gcc
CSTD   = -std=gnu99
CFLAGS = -Wall -Werror -g -pedantic $(CSTD)
LFLAGS = -lm

SOURCES    = $(wildcard *.c)
OBJECTS    = $(patsubst %.c,%.o,$(SOURCES))
EXE        = $(patsubst %.c,%,$(SOURCES))

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $@.o $(LFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(EXE) 
