CC=gcc

all: ex_open ex_cp ex_dup hotel

ex_open: ex_open.o
	$(CC) $< -o $@
ex_cp: ex_cp.o
	$(CC) $< -o $@
ex_dup: ex_dup.o
	$(CC) $< -o $@
hotel: hotel.o
	$(CC) $< -o $@
%.o: %.c
	$(CC) -c $< -o $@
clean:
	rm *.o ex_open ex_cp ex_dup hotel
