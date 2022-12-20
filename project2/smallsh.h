#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define EOL 1
#define ARG 2
#define AMPERSAND 3
#define SEMICOLON 4
#define REDIRECTION 5
#define PIPE 6

#define MAXARG 512
#define MAXBUF 512

#define FOREGROUND 0
#define BACKGROUND 1

struct tok_types {
    int type1;
    int type2;
};
struct sigaction act;
sigjmp_buf position;

int userin(char* p);
void procline();
struct tok_types gettok(char** outptr, struct tok_types types);
int inarg(char c);
int runcommand(char** cline, int where, int narg, struct tok_types types);
char* get_dir(char* dir);
void run_cd(char** cline, int narg);
void int_handler(int signo);
char** run_redirection(char** cline, int narg);
void run_pipe(char** cline, int narg);