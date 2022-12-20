#include "smallsh.h"

static char cwd[255];
static struct passwd *pwd;

char* get_dir(char* dir) {
    if((pwd = getpwuid(getuid())) == NULL){
        perror("getpwuid error");
        return dir;
    }

    char root_home[255] = "/home/";
    char* home = strcat(root_home, pwd->pw_name);
    char* ptr;
    
    if ((ptr = strstr(dir, home))) {
        ptr += strlen(home) - 1;
        dir = ptr;
        strncpy(dir, "~", 1);
    }
    strcat(dir, "$ ");
    return dir;
}

int main(int argc, char* argv[]) {    
    int status;
    char* prompt = get_dir(getcwd(cwd, 255));

    while (userin(prompt) != EOF) {
        procline();
        prompt = get_dir(getcwd(cwd, 255));
        waitpid(-1, &status, 0|WNOHANG);
    }
    return 0;
}
