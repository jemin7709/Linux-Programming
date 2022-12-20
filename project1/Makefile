CC=gcc
CFLAGS=-O

all: shell

shell: main.o smallsh.o 
	$(CC) $(CFLAGS) -o $@ main.o smallsh.o
smallsh.o: smallsh.c
	$(CC) $(CFLAGS) -c -o $@ $<
main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f shell *.o main smallsh 