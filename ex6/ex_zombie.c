#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	pid_t pid;
	if(argc != 3){
		printf("Usage: %s <parent sleep time> <child sleep time>\n", argv[0]);
	}
	int t_parrent_sleep = atoi(argv[1]);
	int t_child_sleep = atoi(argv[2]);
	pid = fork();
	
	if(pid == 0){
		sleep(t_child_sleep);
		printf("Child process [%d] is done, Now child becomes zombie.\n", getpid());
	}
	else if(pid > 0){
		sleep(t_parrent_sleep);
		printf("Parrent process [%d] is done, Now child becomes orphan if it is alive.\n", getpid());
	}
	else{
		perror("fork failed\n");
	}
	return 0;
}
