CC=gcc

all: ex_div0 ex_sigint ex_sigjmp ex_kill

ex_%: ex_%.o
	$(CC) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f *.o ex_div0 ex_sigint ex_sigjmp ex_kill
