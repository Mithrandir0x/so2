DELIVERABLE_N = P1
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

SOURCES    = $(wildcard $(SRC)/*.c)
OBJECTS    = $(patsubst $(SRC)/%.c,$(SRC)/%.o,$(SOURCES))
EXE        = $(patsubst $(SRC)/%.c,$(SRC)/%,$(SOURCES))

all: dist tar

exe: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $@.o $(LFLAGS)

clean:
	rm -rf $(DIST)
	rm -rf $(TEST)
	rm -f $(OBJECTS)
	rm -f $(EXE) 

tests:
	mkdir -p $(TEST)
	$(SRC)/practica1 $(SRC)/fitxer.txt > $(TEST)/log.txt
	valgrind $(SRC)/practica1 $(SRC)/fitxer.txt 2> $(TEST)/valgrind.txt

tar:
	cd $(DIST); tar $(TAR_OPTS) $(PKG).tar.gz $(PKG)

dist: clean exe tests
	mkdir -p $(DIST_SRC)
	mkdir -p $(DIST_TST)
	cp fitxer.txt    $(DIST_SRC)/fitxer.txt
	cp practica1.c   $(DIST_SRC)/practica1.c
	cp fgetc-00.c   $(DIST_SRC)/fgetc-00.c
	cp Makefile      $(DIST_SRC)/Makefile
	cp -r $(TEST)/*  $(DIST_TST)/
