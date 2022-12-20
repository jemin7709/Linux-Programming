#include "sem_header.h"

int main() {
    pid_t pid = getpid();
    int semid = initsem((key_t)pid);
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {
            pid = getpid();
            p(semid);
            printf("Lock: process %d\n", pid);
            sleep(1);
            printf("Unlock: process %d\n", pid);
            v(semid);
            sleep(1);
            printf("Exit: process %d\n", pid);
            return 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    removesem(semid);
}