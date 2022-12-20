CC=gcc
CFLAGS=-O

all: ex_sender ex_sender2 ex_receiver ex_receiver2

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<

ex_%.o: ex_%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o ex_sender ex_sender2 ex_receiver ex_receiver2