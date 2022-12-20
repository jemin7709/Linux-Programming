CC=gcc
CFLAGS=-O

all: ex_ls

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f *.o ex_ls
