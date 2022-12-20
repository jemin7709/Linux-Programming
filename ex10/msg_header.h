#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLEN 100
#define QPERM 0600

struct msg_entry {
    long mtype;
    char mtext[MAXLEN];
};