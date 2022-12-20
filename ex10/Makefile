CC=gcc
CFLAGS=-O

all: msg_sender msg_receiver msg_private

msg_%: msg_%.o
	$(CC) $(CFLAGS) -o $@ $<

msg_%.o: msg_%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o msg_sender msg_receiver msg_private