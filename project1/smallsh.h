#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define EOL 1
#define ARG 2
#define AMPERSAND 3
#define SEMICOLON 4
#define REDIRECTION 5

#define MAXARG 512
#define MAXBUF 512

#define FOREGROUND 0
#define BACKGROUND 1

struct gettok_pair{
    int type;
    int redirection;
};

int userin(char* p);
void procline();
struct gettok_pair gettok(char** outptr);
int inarg(char c);
int runcommand(char** cline, int where, int narg, int redirection);
char* get_dir(char* dir);
void run_cd(char** cline, int narg);