CC=gcc
CFLAGS=-I -g
DEPS=main.c ceva.h

OBJ :=ceva.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o main