CC=gcc
CFLAGS=-I -g

OBJ := src/main.o 
OBJ += src/inserts.o src/removes.o src/utils.o
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

bin/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o src/*.o bin/main