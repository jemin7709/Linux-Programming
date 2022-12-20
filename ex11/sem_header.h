#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};

int initsem(key_t semkey);
void removesem(int semid);
void p(int semid);
void v(int semid);