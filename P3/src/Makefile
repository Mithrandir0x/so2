DELIVERABLE_N = P3
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
FT_HELPERS_OBJECTS = $(patsubst $(SRC)/%.c, $(SRC)/%.o, $(FT_HELPERS_SOURCES))

BUCKET_TESTS = 5 10 50 200
BUCKET_TESTS_FILE = 00ws110.txt

TEST_LIST = llista_prova.cfg

all: practica3

exe: practica3

tests: clean practica3
	mkdir -p $(TEST)
	./practica3 $(TEST_LIST) > $(TEST)/log.txt
	valgrind $(VALGRIND_FLAGS) ./practica3 $(TEST_LIST) 2> $(TEST)/valgrind.txt

practica3: practica3.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS)
	$(CC) -o $@ $@.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(LFLAGS)

unit_tests: clean $(TESTS_EXE)
	valgrind $(VALGRIND_FLAGS) ./test-hashlist-size fitxer.txt 10

plot_tests: $(BUCKET_TESTS)
	echo 'Plotted tests at $(TEST)'

$(BUCKET_TESTS): clean $(TESTS_EXE)
	mkdir -p $(TEST)
	echo '#!/usr/bin/gnuplot' >> $(TEST)/plot_$@.gp
	echo 'reset' >> $(TEST)/plot_$@.gp
	echo 'set terminal png size 1920,720' >> $(TEST)/plot_$@.gp
	echo 'set nokey' >> $(TEST)/plot_$@.gp
	echo 'set boxwidth 0.5' >> $(TEST)/plot_$@.gp
	# echo 'set ytics 0' >> $(TEST)/plot_$@.gp
	echo 'set style fill solid 1.00 noborder' >> $(TEST)/plot_$@.gp
	echo 'set style line 1 lc rgb "blue"' >> $(TEST)/plot_$@.gp
	echo 'set xlabel "Buckets"' >> $(TEST)/plot_$@.gp
	echo 'set ylabel "# Words"' >> $(TEST)/plot_$@.gp
	echo 'plot "$(TEST)/data_$@.dat" using 1:3:xtic(2) with boxes ls 1' >> $(TEST)/plot_$@.gp
	./test-hashlist-size $(BUCKET_TESTS_FILE) $@ > $(TEST)/data_$@.dat
	gnuplot < $(TEST)/plot_$@.gp > $(TEST)/plot_$@.png

$(TESTS_EXE): $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(TESTS_OBJECTS)
	$(CC) -o $@ $(SRC_TEST)/$@.o $(DATA_STRUCTS_OBJECTS) $(FT_HELPERS_OBJECTS) $(LFLAGS)

clean:
	rm -rf $(DIST)
	rm -rf $(TEST)
	rm -rf *.o
	rm -rf *.dat
	rm -f $(EXEMPLES_EXE)
	rm -rf $(SRC_TEST)/*.o
	rm -f $(TESTS_EXE)
	rm -rf practica3

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
	cp practica3.c				$(DIST_SRC)/
	cp $(TEST_LIST)				$(DIST_SRC)/
	cp fitxer.txt				$(DIST_SRC)/
	cp fitxer2.txt				$(DIST_SRC)/
	cp -r $(TEST)/*  			$(DIST_TST)/
