DELIVERABLE_N = P2
PKG           = $(DELIVERABLE_N)_Barrera_Lopez

SRC  = .
TEST = ./../proves
DIST = ./../dist

SRC_TEST = $(SRC)/tests

DIST_PKG = $(DIST)/$(PKG)
DIST_SRC = $(DIST_PKG)/src
DIST_TST = $(DIST_PKG)/proves

TAR_OPTS      = zcvf

CC     = gcc
CSTD   = -std=gnu99
CFLAGS = -Wall -Werror -g -pedantic $(CSTD)
LFLAGS = -lm

VALGRIND_FLAGS = --leak-check=full -v

EXEMPLES_SOURCES    = $(wildcard $(SRC)/exemple-*.c)
EXEMPLES_OBJECTS    = $(patsubst $(SRC)/exemple-%.c, $(SRC)/exemple-%.o, $(EXEMPLES_SOURCES))
EXEMPLES_EXE        = $(patsubst $(SRC)/exemple-%.c, $(SRC)/exemple-%, $(EXEMPLES_SOURCES))

TESTS_SOURCES    = $(wildcard $(SRC_TEST)/*.c)
TESTS_OBJECTS    = $(patsubst $(SRC_TEST)/%.c, $(SRC_TEST)/%.o, $(TESTS_SOURCES))
TESTS_EXE        = $(patsubst $(SRC_TEST)/%.c, $(SRC)/%, $(TESTS_SOURCES))

DATA_STRUCTS_SOURCES = $(SRC)/linked-list.c $(SRC)/red-black-tree.c $(SRC)/hash-list.c
DATA_STRUCTS_OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(DATA_STRUCTS_SOURCES))

all: dist tar

test_unit_tests:
	echo $(TESTS_SOURCES)
	echo $(TESTS_OBJECTS)
	echo $(TESTS_EXE)

unit_tests: clean $(TESTS_EXE)
	valgrind ./test-hash-lists $(VALGRIND_FLAGS)

$(TESTS_EXE): $(TESTS_OBJECTS) $(DATA_STRUCTS_OBJECTS)
	$(CC) -o $@ $(SRC_TEST)/$@.o $(DATA_STRUCTS_OBJECTS) $(LFLAGS)

# exemples: $(EXEMPLES_EXE)

# $(EXEMPLES_EXE): $(EXEMPLES_OBJECTS) $(DATA_STRUCTS_OBJECTS)
#	$(CC) -o $@ $@.o $(DATA_STRUCTS_OBJECTS) $(LFLAGS)

clean:
	rm -rf $(DIST)
	rm -rf $(TEST)
	rm -rf *.o
	rm -f $(EXEMPLES_EXE)
	rm -rf $(SRC_TEST)/*.o
	rm -f $(TESTS_EXE)

tar:
	cd $(DIST); tar $(TAR_OPTS) $(PKG).tar.gz $(PKG)

dist: clean exe tests
	echo "Not implemented yet."


