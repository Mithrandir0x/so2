CC     = gcc
CFLAGS = -Wall -Werror -g
LFLAGS = -lm

EXEMPLES_C = $(wildcard exemple-*.c)
EXEMPLES_O = $(patsubst %.c,%.o,$(EXEMPLES_C))
EXEMPLES   = $(patsubst %.c,%,$(EXEMPLES_C))

$(EXEMPLES): $(EXEMPLES_O)
	$(CC) $@.o -o $@ $(LFLAGS)

$(EXEMPLES_O): $(EXEMPLES_C)
	$(CC) $(CFLAGS) -c $^

exemples: $(EXEMPLES)

all:

clean:
	rm -f $(EXEMPLES_O)
	rm -f $(EXEMPLES) 
