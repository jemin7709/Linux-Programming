CC=gcc
CFLAGS=-O

all: ex_exec ex_exit ex_zombie

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o ex_exec ex_exit ex_zombie
