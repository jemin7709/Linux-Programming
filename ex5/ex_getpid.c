#include <unistd.h>
#include <stdio.h>

int main(){
	pid_t pid;
	pid_t ppid;

	pid = getpid();
	ppid = getppid();

	printf("PID: %d, PPID: %d", pid, ppid);

	sleep(10);

	return 0;
}
