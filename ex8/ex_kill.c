#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int opt;
	int flag_g = 0;
	int pid;
	int signal_num;
	
	while((opt = getopt(argc, argv, "g")) != -1){
		switch(opt){
			case 'g':
				flag_g = 1;
				break;
			default:
				printf("Unavailable option\n");
				return 1;
		}
	}
	if((argc - optind) != 2){
		printf("argument err\n");
		return 1;
	}	
	signal_num = atoi(argv[optind]);
	pid = atoi(argv[optind + 1]);
	if(flag_g) pid = -1 * pid;
	kill(pid, signal_num);

	return 0;
}
