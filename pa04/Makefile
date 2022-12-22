CC:= gcc
CFLAGS:= -lgmp -I ~/.local/include/pbc -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc

all: pa04.o test

pa04.o: pa04.c
	$(CC) $^ -o $@ $(CFLAGS)
	
test: test.c
	$(CC) $^ -o $@ $(CFLAGS)
	
clean:
	rm test pa04.o
