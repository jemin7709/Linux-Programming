#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ftw.h>
#include <sys/sysmacros.h>

int flag_a, flag_l, flag_R = 0;

static short octarray_type[6] = {0120000, 0100000, 0010000,
				 0020000, 0040000, 0060000};
static short octarray[9] = {0400, 0200, 0100,
			    0040, 0020, 0010,
			    0004, 0002, 0001};

static char perms_type[6] = "l-pcdb";
static char perms[10] = "rwxrwxrwx";

void print_perm(mode_t perm){
	char p[11];
	for(int i = 0; i < 6; i++){
		if(perm & octarray_type[i]){
			p[0] = perms_type[i];
		}
	}
	for(int i = 1; i < 10; i++){
		if(perm & octarray[i - 1]){
			p[i] = perms[i - 1];
		} else { 
			p[i] = '-';
		}
	}
	p[10] = '\0';
	printf("%s", p);
}		

int check_device(mode_t perm){
	if(perm & 0020000 || perm & 0040000){
		return 1;
	}
	return 0;
}

void list(const char* dir_path){
	DIR* dirp;
	struct dirent* entry;

	if((dirp = opendir(dir_path)) == NULL){
		perror("Directory cannot be opened\n");
	}

	while((entry = readdir(dirp)) != NULL){
	       if(flag_a || entry->d_name[0] != '.'){
			printf("%-15s\t", entry->d_name);      
	      }
	}
	printf("\n");
	closedir(dirp);
}

void list_long(const char* dir_path){
	DIR* dirp;
	struct dirent* entry;
	struct stat buf;
	char prev_dir[512];

	if(getcwd(prev_dir, 512) == NULL) perror("getcwd error");
	if((dirp = opendir(dir_path)) == NULL) perror("Directory open error");
	if(chdir(dir_path) != 0) perror("chdir error");

	while((entry = readdir(dirp)) != NULL){
		if(flag_a || entry->d_name[0] != '.'){
			if(stat(entry->d_name, &buf) == -1) continue;
			//printf("%3o\t", buf.st_mode & 0777);
			print_perm(buf.st_mode);
			printf("%3ld\t", buf.st_nlink);
			printf("%6d\t", buf.st_uid);
			printf("%6d\t", buf.st_gid);
			if(check_device(buf.st_mode)){
				printf("%6u, %6u\t", major(buf.st_rdev), minor(buf.st_rdev));
			}
			else{
				printf("%12ld\t", buf.st_size);
			}
			printf("%-15s\t", entry->d_name);
			printf("\n");
		}
	}
	closedir(dirp);
	chdir(prev_dir);
}

int list_recursive(const char* path, const struct stat* buf, int type){
	if(type == FTW_D){
		printf("%s:\n", path);
		if(flag_l) list_long(path);
		else list(path);
		printf("\n");
	}
	return 0;
}

int main(int argc, char* argv[]){
	int opt;
	while((opt = getopt(argc, argv, "alR")) != -1){
		switch(opt){
			case 'a':
				flag_a = 1;
				break;
			case 'l':
				flag_l = 1;
				break;
			case 'R':
				flag_R = 1;
				break;
			default:
				printf("Wrong usage\n");
				return -1;
		}
	}
	if(argc - optind == 0){
		if(flag_R) ftw("./", list_recursive, 10);
		else if(flag_l ) list_long("./");
		else list("./");
	}else if(argc - optind == 1){
		if(flag_R) ftw(argv[optind], list_recursive, 10);
		else if(flag_l) list_long(argv[optind]);
		else list(argv[optind]);
	}else{
		for(int i = optind; i < argc; i++){
			printf("%s:\n", argv[i]);
			if(flag_R) ftw(argv[i], list_recursive, 10);
			else if(flag_l) list_long(argv[i]);
			else list(argv[i]);
		}
	}
	return 0;
}
