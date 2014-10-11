DELIVERABLE_N = P2
PKG           = $(DELIVERABLE_N)_Barrera_Lopez

SRC  = .
TEST = ./../proves
DIST = ./../dist

DIST_PKG = $(DIST)/$(PKG)
DIST_SRC = $(DIST_PKG)/src
DIST_TST = $(DIST_PKG)/proves

TAR_OPTS      = zcvf

CC     = gcc
CSTD   = -std=gnu99
CFLAGS = -Wall -Werror -g -pedantic $(CSTD)
LFLAGS = -lm

EXEMPLES_SOURCES    = $(wildcard $(SRC)/exemple-*.c)
EXEMPLES_OBJECTS    = $(patsubst $(SRC)/exemple-%.c, $(SRC)/exemple-%.o, $(EXEMPLES_SOURCES))
EXEMPLES_EXE        = $(patsubst $(SRC)/exemple-%.c, $(SRC)/exemple-%, $(EXEMPLES_SOURCES))

DATA_STRUCTS_SOURCES = $(SRC)/linked-list.c $(SRC)/red-black-tree.c
DATA_STRUCTS_OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(DATA_STRUCTS_SOURCES))

all: dist tar

exemples: $(EXEMPLES_EXE)

$(EXEMPLES_EXE): $(EXEMPLES_OBJECTS) $(DATA_STRUCTS_OBJECTS)
	$(CC) -o $@ $@.o $(DATA_STRUCTS_OBJECTS) $(LFLAGS)

clean:
	rm -rf $(DIST)
	rm -rf $(TEST)
	rm -f *.o
	rm -f $(EXEMPLES_EXE)

tests:
	mkdir -p $(TEST)
	echo "No tests available yet."

tar:
	cd $(DIST); tar $(TAR_OPTS) $(PKG).tar.gz $(PKG)

dist: clean exe tests
	echo "Not implemented yet."
