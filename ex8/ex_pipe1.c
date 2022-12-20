#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 64

int main(){
	int fd[2];
	char buf[BUFSIZE];
	
	if(pipe(fd) == -1){
		perror("pipe");
		return 0;
	}
	switch(fork()){
		case -1:
			perror("fork");
			return 0;
		case 0:
			close(fd[0]);
			write(fd[1], "Hello, I'm child.", BUFSIZE);
			return 0;
		default:
			close(fd[1]);
			read(fd[0], buf, BUFSIZE);
			printf("Message:, %s\n", buf);
			wait(NULL);
	}
	return 0;
}
