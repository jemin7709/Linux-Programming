#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void child_handler(int a){
	waitpid(-1, NULL, 0);
}

int main(){
	struct sigaction act;
	act.sa_handler = child_handler;
	sigfillset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);

	fork();
	fork();

	for(;;);
	return 0;
}
