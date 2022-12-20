#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void print_usage(char* prog_name){
	printf("Default usage: %s <filename> <model>\n", prog_name);
	printf("-- Change the permission of the <filename> to <mode>\n");
	printf("    -v: output a diagnostic for every file processed\n");
	printf("    -h: output a usage of ex_chmod\n");
}

int main(int argc, char* argv[]){
	int opt;
	int flag_v = 0;
	while((opt = getopt(argc, argv, "vh")) != -1){
		switch(opt){
			case 'v':
				flag_v = 1; 
				break;
			case 'h':
				print_usage(argv[0]);
				return -1;
			default:
				print_usage(argv[0]);
				return -1;
		}
	}

	mode_t newmode;
	if(argc - optind <= 1){
		printf("Missing operand\n");
		printf("Try 'ex_chmod -h' for more information\n");
		return -1;
	}
	
	if(access(argv[optind + 1], F_OK) != 0){
		perror("file is not existed");
		return -1;
	}
	
	sscanf(argv[optind], "%o", &newmode);
	
	if(chmod(argv[optind + 1], newmode) == -1){
		perror("mode change failed");
		return -1;
	}
	if(flag_v){
		printf("mode of %s is changed to %o\n", argv[optind + 1], newmode);
	}
	return 0;
}
