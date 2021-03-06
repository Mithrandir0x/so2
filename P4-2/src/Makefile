##
#  Project utilities and paths
##

DELIVERABLE_N = P4-2
PKG           = $(DELIVERABLE_N)_Barrera_Lopez

SRC  = .
TEST = ./../proves
DIST = ./../dist

SRC_TEST = $(SRC)/tests

DIST_PKG = $(DIST)/$(PKG)
DIST_SRC = $(DIST_PKG)/src
DIST_TST = $(DIST_PKG)/proves

UNAME = $(shell sh -c 'uname -s 2>/dev/null || echo not')

##
#  Database path
##

DATABASE = ~/Dropbox/Universitat/Sistemes\ Operatius\ 2/base_dades
DATABASE_PATHS = $(DATABASE)/llista.cfg \
	$(DATABASE)/etext00 \
	$(DATABASE)/etext01 \
	$(DATABASE)/etext02 \
	$(DATABASE)/etext03 \
	$(DATABASE)/etext04 \
	$(DATABASE)/etext05 \
	$(DATABASE)/etext90 \
	$(DATABASE)/etext91 \
	$(DATABASE)/etext92 \
	$(DATABASE)/etext93 \
	$(DATABASE)/etext94 \
	$(DATABASE)/etext95 \
	$(DATABASE)/etext96 \
	$(DATABASE)/etext97 \
	$(DATABASE)/etext98 \
	$(DATABASE)/etext99 \

##
#  Distribution thingies
##

TAR_OPTS      = zcvf

##
#  Compiler/Linker configuration and valgrind flags
##

CC     = clang
CSTD   = -std=gnu99
CFLAGS = -Wall -Werror -g -pedantic $(CSTD) -fblocks
LFLAGS = -lm -lBlocksRuntime -lpthread
ifeq ($(UNAME),Darwin)
LFLAGS = -lm
endif

VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes -v
HELGRIND_FLAGS = --tool=helgrind -v

##
#  Project dependencies
##

DATA_STRUCTS = linked-list red-black-tree hash-list stats
FT_HELPERS   = word-utils config persistence

##
#  Compilation Targets and Executable generation
##

TESTS_SOURCES    = $(wildcard $(SRC_TEST)/*.c)
TESTS_OBJECTS    = $(patsubst $(SRC_TEST)/%.c, $(SRC_TEST)/%.o, $(TESTS_SOURCES))
TESTS_EXE        = $(patsubst $(SRC_TEST)/%.c, $(SRC)/%, $(TESTS_SOURCES))

DATA_STRUCTS_SOURCES = $(patsubst %, $(SRC)/%.c, $(DATA_STRUCTS))
DATA_STRUCTS_HEADERS = $(patsubst %, $(SRC)/%.h, $(DATA_STRUCTS))
DATA_STRUCTS_OBJECTS = $(patsubst %, $(SRC)/%.o, $(DATA_STRUCTS))

FT_HELPERS_SOURCES = $(patsubst %, $(SRC)/%.c, $(FT_HELPERS))
FT_HELPERS_HEADERS = $(patsubst %, $(SRC)/%.h, $(FT_HELPERS))
FT_HELPERS_OBJECTS = $(patsubst %, $(SRC)/%.o, $(FT_HELPERS))

##
#  Test parameters and target paths
##

BUCKET_TESTS = 5 10 50 200
BUCKET_TESTS_FILE = 00ws110.txt

TEST_LIST = llista_prova.cfg

PROD_LIST = $(DATABASE)/llista.cfg

all: practica4

exe: practica4

# tests: clean practica4
# 	mkdir -p $(TEST)
# 	./practica4 $(TEST_LIST) > $(TEST)/log.txt
# 	valgrind $(VALGRIND_FLAGS) ./practica4

setup_database: $(DATABASE_PATHS)
	ln -s $(DATABASE)/llista.cfg $(SRC)/llista.cfg
	ln -s $(DATABASE)/etext00 $(SRC)/etext00
	ln -s $(DATABASE)/etext01 $(SRC)/etext01
	ln -s $(DATABASE)/etext02 $(SRC)/etext02
	ln -s $(DATABASE)/etext03 $(SRC)/etext03
	ln -s $(DATABASE)/etext04 $(SRC)/etext04
	ln -s $(DATABASE)/etext05 $(SRC)/etext05
	ln -s $(DATABASE)/etext90 $(SRC)/etext90
	ln -s $(DATABASE)/etext91 $(SRC)/etext91
	ln -s $(DATABASE)/etext92 $(SRC)/etext92
	ln -s $(DATABASE)/etext93 $(SRC)/etext93
	ln -s $(DATABASE)/etext94 $(SRC)/etext94
	ln -s $(DATABASE)/etext95 $(SRC)/etext95
	ln -s $(DATABASE)/etext96 $(SRC)/etext96
	ln -s $(DATABASE)/etext97 $(SRC)/etext97
	ln -s $(DATABASE)/etext98 $(SRC)/etext98
	ln -s $(DATABASE)/etext99 $(SRC)/etext99

valgrind: practica4
	valgrind $(VALGRIND_FLAGS) ./practica4

helgrind: practica4
	valgrind $(HELGRIND_FLAGS) ./practica4

practica4: practica4.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS)
	$(CC) -o $@ $@.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(LFLAGS)

unit_tests: $(TESTS_EXE)
	# valgrind $(VALGRIND_FLAGS) ./test-hashlist-size fitxer.txt 10
	# valgrind $(VALGRIND_FLAGS) ./test-persistence
	# ./test-persistence
	# valgrind $(VALGRIND_FLAGS) ./test-mt-config llista.cfg 4

$(TESTS_EXE): $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(TESTS_OBJECTS)
	$(CC) -o $@ $(SRC_TEST)/$@.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(LFLAGS)

clean:
	rm -f llista.cfg etext00 etext01 etext02 etext03 etext04 etext05 etext90 etext91 etext92 etext93 etext94 etext95 etext96 etext97 etext98 etext99
	rm -rf $(DIST)
	rm -rf $(TEST)
	rm -rf *.o
	rm -rf *.dat
	rm -rf *.png
	rm -rf *.rbt
	rm -f $(EXEMPLES_EXE)
	rm -rf $(SRC_TEST)/*.o
	rm -f $(TESTS_EXE)
	rm -rf practica4

tar:
	cd $(DIST); tar $(TAR_OPTS) $(PKG).tar.gz $(PKG)

dist: clean exe tests
	mkdir -p $(DIST_SRC)
	cp $(DATA_STRUCTS_SOURCES)  $(DIST_SRC)/
	cp $(DATA_STRUCTS_HEADERS)  $(DIST_SRC)/
	cp $(FT_HELPERS_SOURCES)    $(DIST_SRC)/
	cp $(FT_HELPERS_HEADERS)    $(DIST_SRC)/
	cp Makefile                 $(DIST_SRC)/
	cp practica4.c              $(DIST_SRC)/
	cp $(TEST_LIST)             $(DIST_SRC)/
	cp fitxer.txt               $(DIST_SRC)/
	cp fitxer2.txt              $(DIST_SRC)/

mk_tst_targets:
	echo $(DATA_STRUCTS_SOURCES)
	echo $(DATA_STRUCTS_HEADERS)
	echo $(DATA_STRUCTS_OBJECTS)
	echo $(FT_HELPERS_SOURCES)
	echo $(FT_HELPERS_HEADERS)
	echo $(FT_HELPERS_OBJECTS)
