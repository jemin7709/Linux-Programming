#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/select.h>
#include <setjmp.h>
#define BUFSIZE 64

fd_set readfds;
int nfds = 3;
sigjmp_buf position;

void usr1_handler(int signo, siginfo_t *siginfo, void *param2){
	char fifo_name[BUFSIZE];
	sprintf(fifo_name, "FIFO_%d", siginfo->si_pid);
	printf("%s\n", fifo_name);
	mkfifo(fifo_name, 0666);

	int fd = open(fifo_name, O_RDONLY);
	FD_SET(fd, &readfds);
	if(fd >= nfds) nfds = fd + 1;
	siglongjmp(position, 1);
}

int main(int argc, char* argv[]){
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_sigaction = usr1_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	char buf[BUFSIZE];
	int nread;

	printf("PID: %d\n", getpid());

	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	fd_set fds;
	
	sigsetjmp(position, 1);
	while(fds = readfds, select(nfds, &fds, NULL, NULL, NULL) > 0){
		if(FD_ISSET(0, &fds)) read(0, buf, 1);
		for(int i=3;i<nfds;i++){
			if(FD_ISSET(i, &fds)){
				if((nread = read(i, buf, BUFSIZE)) > 0){
					printf("\nMsg received: \n");
					write(1, buf, nread);
				}
				if(nread == 0){
					FD_CLR(i, &readfds);
					close(i);
				}
			}
		}
	}
	return 0;
}


