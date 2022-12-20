#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFSIZE 64

int main(int argc, char* argv[]) {
    char buf[BUFSIZE];
    int fd;
    int nread;
    if (argc != 2) {
        printf("Usage: %s <FIFO NAME>", argv[0]);
        return 1;
    }
    if (mkfifo(argv[1], 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            return 2;
        }
    }
    if ((fd = open(argv[1], O_RDWR)) < 0) {
        perror("open");
        return 3;
    }
    while ((nread = read(fd, buf, BUFSIZE)) > 0) {
        printf("\nMsg received: \n");
        if(write(1, buf, nread) == -1) perror("write");
        return 0;
    }
}