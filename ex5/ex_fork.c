#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	pid_t pid;
	printf("Calling fork...\n");

	pid = fork();

	if(pid == 0){
		printf("I'm the child. pid: %d, getpid(): %d, getppid(): %d\n", pid, getpid(), getppid());
		sleep(9);
	}
	else if(pid > 0){
		printf("I'm the parent. pid: %d, getpid(): %d, getppid(): %d\n", pid, getpid(), getppid());
		sleep(9);
	}
	else{
		printf("fork failed\n");
	}
	return 0;
}
