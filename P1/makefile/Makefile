# This is the makefile that generates the executable

CFLAGS = -Wall -Werror -g
LFLAGS = -lm

FILES_C = $(wildcard *.c)

TARGET = practica1

# There is no need to change the instructions below this
# line

FILES_O = $(subst .c,.o,$(FILES_C))

$(TARGET): $(FILES_O) Makefile
	gcc $(FILES_O) -o $(TARGET) $(LFLAGS) 

%.o: %.c Makefile 
	gcc $(CFLAGS) -c $<

all: $(TARGET) 

clean:
	rm -f $(FILES_O) $(TARGET) 
