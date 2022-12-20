#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc != 2) return 0;
	int pid = atoi(argv[1]);
	kill(pid, SIGINT);
	return 0;
}
