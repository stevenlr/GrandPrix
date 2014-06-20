CC=gcc
# CFLAGS=-Wall -pedantic -ansi -std=c99 -g -DDEBUG
CFLAGS=-Wall -pedantic -ansi -std=c99 -O3
LDFLAGS=-lm
BIN=pilote_DogeCorn
SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

.PHONY: all clean depend

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -f *.o

depend.d: depend

depend:
	$(CC) *.c -E -MM > depend.d

include depend.d
