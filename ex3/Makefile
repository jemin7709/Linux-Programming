CC = gcc
CFLAGS = -O

all: ex_umask ex_getopt ex_chmod

ex_%: ex_%.o
	$(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f *.o ex_umask ex_getopt ex_chmod
