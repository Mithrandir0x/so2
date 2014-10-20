DELIVERABLE_N = P2
PKG           = $(DELIVERABLE_N)_Barrera_Lopez

SRC  = .
TEST = ./../proves
DIST = ./../dist

SRC_TEST = $(SRC)/tests

DIST_PKG = $(DIST)/$(PKG)
DIST_SRC = $(DIST_PKG)/src
DIST_TST = $(DIST_PKG)/proves

UNAME = $(shell sh -c 'uname -s 2>/dev/null || echo not')

TAR_OPTS      = zcvf

CC     = clang
CSTD   = -std=gnu99
CFLAGS = -Wall -Werror -g -pedantic $(CSTD) -fblocks
LFLAGS = -lm -lBlocksRuntime
ifeq ($(UNAME),Darwin)
LFLAGS = -lm
endif

VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes -v

EXEMPLES_SOURCES    = $(wildcard $(SRC)/exemple-*.c)
EXEMPLES_OBJECTS    = $(patsubst $(SRC)/exemple-%.c, $(SRC)/exemple-%.o, $(EXEMPLES_SOURCES))
EXEMPLES_EXE        = $(patsubst $(SRC)/exemple-%.c, $(SRC)/exemple-%, $(EXEMPLES_SOURCES))

TESTS_SOURCES    = $(wildcard $(SRC_TEST)/*.c)
TESTS_OBJECTS    = $(patsubst $(SRC_TEST)/%.c, $(SRC_TEST)/%.o, $(TESTS_SOURCES))
TESTS_EXE        = $(patsubst $(SRC_TEST)/%.c, $(SRC)/%, $(TESTS_SOURCES))

DATA_STRUCTS_SOURCES = $(SRC)/linked-list.c $(SRC)/red-black-tree.c $(SRC)/hash-list.c
DATA_STRUCTS_HEADERS = $(patsubst $(SRC)/%.c, $(SRC)/%.h, $(DATA_STRUCTS_SOURCES))
DATA_STRUCTS_OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(DATA_STRUCTS_SOURCES))

FT_HELPERS_SOURCES = $(SRC)/word-utils.c $(SRC)/config.c
FT_HELPERS_HEADERS = $(patsubst $(SRC)/%.c, $(SRC)/%.h, $(FT_HELPERS_SOURCES))
FT_HELPERS_OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(FT_HELPERS_SOURCES))

all: dist tar

exe: practica2

tests: clean practica2
	valgrind $(VALGRIND_FLAGS) ./practica2

practica2: practica2.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS)
	$(CC) -o $@ $@.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(LFLAGS)

unit_tests: clean $(TESTS_EXE)
	# valgrind $(VALGRIND_FLAGS) ./test-blocks
	# valgrind $(VALGRIND_FLAGS) ./test-hash-lists
	# valgrind $(VALGRIND_FLAGS) ./test-word-extract

$(TESTS_EXE): $(TESTS_OBJECTS) $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS)
	$(CC) -o $@ $(SRC_TEST)/$@.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(LFLAGS)

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
	rm -rf practica2

tar:
	cd $(DIST); tar $(TAR_OPTS) $(PKG).tar.gz $(PKG)

dist: clean exe tests
	mkdir -p $(DIST_SRC)
	mkdir -p $(DIST_TST)
	cp $(DATA_STRUCTS_SOURCES)	$(DIST_SRC)/
	cp $(DATA_STRUCTS_HEADERS)	$(DIST_SRC)/
	cp $(FT_HELPERS_SOURCES)	$(DIST_SRC)/
	cp $(FT_HELPERS_HEADERS)	$(DIST_SRC)/
	cp Makefile					$(DIST_SRC)/
	cp practica2.c				$(DIST_SRC)/
