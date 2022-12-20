#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFSIZE 64

char fifo_name[BUFSIZE];
int fd;

void int_handler(int signo) {
    close(fd);
    if (unlink(fifo_name) != 0) perror("unlink");
    exit(0);
}

int main(int argc, char* argv[]) {
    char buf[BUFSIZE];
    int nread;
    pid_t pid_receiver;
    if (argc < 2) {
        printf("Usage: %s <RECEIVER PID>\n", argv[0]);
        return 1;
    }
    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_handler = int_handler;
    sigaction(SIGINT, &act, NULL);

    sprintf(fifo_name, "FIFO_%d", getpid());
    mkfifo(fifo_name, 0666);
    printf("FIFO_NAME: %s\n", fifo_name);

    pid_receiver = atoi(argv[1]);
    kill(pid_receiver, SIGUSR1);

    if ((fd = open(fifo_name, O_WRONLY)) < 0) {
        perror("open");
        return 2;
    }
    while ((nread = read(0, buf, BUFSIZE)) > 0) {
        if (write(fd, buf, nread) != nread) {
            perror("sender write");
            return 3;
        }
    }
    perror("stdin read");
    return 4;
}
