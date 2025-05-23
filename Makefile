CC=gcc
CFLAGS=-I.
DEPS = tp.h
OBJ = main.o tp.o dinamica.o heuristica.o
EXEC = tp2

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core