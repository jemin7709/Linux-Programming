CC=gcc
CFLAGS=-O

all: ex_kill ex_test ex_pipe1 ex_pipe2 ex_pipe3

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o ex_kill ex_test ex_pipe1 ex_pipe2 ex_pipe3
 
