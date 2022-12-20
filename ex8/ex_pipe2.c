#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define BUFSIZE 64

int main(){
	int p2c_fd[2];
	int c2p_fd[2];
	char buf[BUFSIZE];

	if(pipe(p2c_fd) == -1){
		perror("parent to child pipe");
		return 0;
	}
	if(pipe(c2p_fd) == -1){
		perror("child to parent pipe");
		return 0;
	}
	
	switch(fork()){
		case -1:
			perror("fork");
			return 0;
		case 0:
			close(p2c_fd[1]);
			close(c2p_fd[0]);
			write(c2p_fd[1], "Hello, I'm child.", BUFSIZE);
			read(p2c_fd[0], buf, BUFSIZE);
			printf("Message from parent: %s\n", buf);
			return 0;
		default:
			close(p2c_fd[0]);
			close(c2p_fd[1]);
			read(c2p_fd[0], buf, BUFSIZE);
			printf("Message from child: %s\n", buf);
			sleep(5);
			write(p2c_fd[1], "Hello, I'm your parent", BUFSIZE);
			wait(NULL);
			return 0;
	}
}
