#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void int_handler(int a){
	printf("\nSIGINT caught\n");
}

int main(){
	struct sigaction act;
	sigaddset(&act.sa_mask, SIGINT);
	act.sa_handler = int_handler;
	sigaction(SIGINT, &act, NULL);
	printf("int_hander() for SIGINT is registered\n");

	for(int i=0; i<4; i++){
		printf("sleep call #%d\n", i);
		sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);
		sleep(3);
		sigprocmask(SIG_UNBLOCK, &act.sa_mask, NULL);
	}
	return 0;
}
