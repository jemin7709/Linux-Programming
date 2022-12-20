#include "smallsh.h"

static char inpbuf[MAXBUF];
static char tokbuf[2 * MAXBUF];
static char *ptr = inpbuf;
static char *tok = tokbuf;

static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

static struct gettok_pair pair;
static struct passwd *pwd;

int userin(char *p) {
    int c, count;
    ptr = inpbuf;
    tok = tokbuf;

    printf("%s", p);
    count = 0;

    while (1) {
        if ((c = getchar()) == EOF) return EOF;
        if (count < MAXBUF) inpbuf[count++] = c;
        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            return count;
        }
        if (c == '\n' || count >= MAXBUF) {
            printf("smallsh: input line too long\n");
            count = 0;
            printf("%s", p);
        }
    }
}

struct gettok_pair gettok(char **outptr) {
    *outptr = tok;
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    *tok++ = *ptr;
    switch (*ptr++) {
        case '\n': 
            pair.type = EOL; 
            break;
        case '&': 
            pair.type = AMPERSAND; 
            break;
        case ';': 
            pair.type = SEMICOLON; 
            break;
        case '>':
            pair.redirection = REDIRECTION;
        default:
            pair.type = ARG;
            while (inarg(*ptr)) *tok++ = *ptr++;
    }
    *tok++ = '\0';
    return pair;
}

int inarg(char c) {
    char *wrk;

    for (wrk = special; *wrk; wrk++) {
        if (c == *wrk) return 0;
    }
    return 1;
}

void procline() {
    char *arg[MAXARG + 1];
    int toktype, type;
    int narg = 0;
    for (;;) {
        gettok(&arg[narg]);
        switch (toktype = pair.type) {
            case ARG:
                if (narg < MAXARG) narg++;
                break;
            case EOL:
            case SEMICOLON:
            case AMPERSAND:
                if (toktype == AMPERSAND) type = BACKGROUND;
                else type = FOREGROUND;
                if (narg != 0) {
                    arg[narg] = NULL;
                    runcommand(arg, type, narg, pair.redirection);
                    pair.redirection = 0;
                }
                if (toktype == EOL) return;
                narg = 0;
                break;
        }
    }
}

void run_cd(char** cline, int narg) { 
    if((pwd = getpwuid(getuid())) == NULL){
        perror("getpwuid error");
        return ;
    }
    
    if (narg > 1) {
        char home[255] = "/home/";
        char* dir = *(cline + 1);
        strcat(home, pwd->pw_name);
        
        if (strstr(dir, "~")){
            strcat(home, dir + 1);
            dir = home;
        }
        if(chdir(dir) == -1) {
            if(errno == EACCES){
                printf("%s: Permission denied\n", dir);
            }
        }
    } else if(chdir(pwd->pw_dir) == -1) {
        printf("chdir error\n");
    }
}

int runcommand(char **cline, int where, int narg, int redirection) {    
    char *temp[MAXARG + 1];
    pid_t pid;
    int status;
    int fd;
    int i;
    if (strstr(*cline, "cd")) {
        if(narg > 2){
            printf("cd: too many arguments\n");
            return -1;
        }
        run_cd(cline, narg);
        return 0;
    }
    if(strstr(*cline, "exit")){
        exit(0);
    }
    switch (pid = fork()) {
        case -1: 
            perror("smallsh"); 
            return -1;
        case 0:
            setpgid(getpid(), 0);
            if(redirection) {
                for(i = 0; !strstr(*(cline + i), ">"); i++) temp[i] = *(cline + i);
                fd = open(*(cline + i + 1), O_WRONLY|O_CREAT|O_TRUNC, 0644);
                fcntl(fd, F_SETFD, 1);
                dup2(fd, 1);
                cline = temp;
            }
            execvp(*cline, cline);
            perror(*cline);
            exit(1);
    }
    if (where == BACKGROUND) {
        printf("[Process id] %d\n", pid);
        return 0;
    }
    if (waitpid(pid, &status, 0) == -1) return -1;
    else return status;
}
