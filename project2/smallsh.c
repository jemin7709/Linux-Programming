#include "smallsh.h"

static char inpbuf[MAXBUF];
static char tokbuf[2 * MAXBUF];
static char* ptr = inpbuf;
static char* tok = tokbuf;

static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

static struct passwd* pwd;

void int_handler(int signo) {
    putchar('\n');
    siglongjmp(position, 1);
}

char* get_dir(char* dir) {
    if ((pwd = getpwuid(getuid())) == NULL) {
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

int userin(char* p) {
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

struct tok_types gettok(char** outptr, struct tok_types types) {
    *outptr = tok;
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    *tok++ = *ptr;
    switch (*ptr++) {
        case '\n':
            types.type1 = EOL;
            break;
        case '&':
            types.type1 = AMPERSAND;
            break;
        case ';':
            types.type1 = SEMICOLON;
            break;
        case '>':
            types.type1 = REDIRECTION;
            types.type2 = REDIRECTION;
            break;
        case '|':
            types.type2 = PIPE;
        default:
            types.type1 = ARG;
            while (inarg(*ptr)) *tok++ = *ptr++;
    }
    *tok++ = '\0';
    return types;
}

int inarg(char c) {
    char* wrk;

    for (wrk = special; *wrk; wrk++) {
        if (c == *wrk) return 0;
    }
    return 1;
}

void procline() {
    struct tok_types types;
    char* arg[MAXARG + 1];
    int toktype, where;
    int narg = 0;
    for (;;) {
        types = gettok(&arg[narg], types);
        switch (toktype = types.type1) {
            case ARG:
                if (narg < MAXARG) narg++;
                break;
            case EOL:
            case SEMICOLON:
            case AMPERSAND:
                if (toktype == AMPERSAND) where = BACKGROUND;
                else where = FOREGROUND;
                if (narg != 0) {
                    arg[narg] = NULL;
                    runcommand(arg, where, narg, types);
                    types.type2 = 0;
                }
                if (toktype == EOL) return;
                narg = 0;
                break;
        }
    }
}

void run_cd(char** cline, int narg) {
    if ((pwd = getpwuid(getuid())) == NULL) {
        perror("getpwuid error");
        return;
    }

    if (narg > 1) {
        char home[255] = "/home/";
        char* dir = *(cline + 1);
        strcat(home, pwd->pw_name);

        if (!strcmp(dir, "~")) {
            strcat(home, dir + 1);
            dir = home;
        }
        if (chdir(dir) == -1) {
            if (errno == EACCES) {
                printf("%s: Permission denied\n", dir);
            }
        }
    } else if (chdir(pwd->pw_dir) == -1) {
        printf("chdir error\n");
    }
}

char** run_redirection(char** cline, int narg) {
    int fd, i = 0;
    fd = open(cline[narg - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    fcntl(fd, F_SETFD, 1);
    dup2(fd, 1);
    cline[narg - 1] = NULL;
    return cline;
}

void run_pipe(char** cline, int narg) {
    char *cmd1[MAXBUF] = {NULL}, *cmd2[MAXBUF] = {NULL};
    int i = 0, j = 0;
    int p[2];

    for (i; strcmp(cline[i], "|"); i++) cmd1[i] = cline[i];
    for (i = i + 1; i < narg; i++) cmd2[j++] = cline[i];
    if ((pipe(p) == -1)) perror("pipe");
    else {
        switch (fork()) {
            case -1:
                perror("pipe fork");
            case 0:
                dup2(p[0], 0);
                close(p[0]);
                close(p[1]);
                execvp(*cmd2, cmd2);
                perror(*cmd2);
                exit(1);
            default:
                dup2(p[1], 1);
                close(p[0]);
                close(p[1]);
                execvp(*cmd1, cmd1);
                perror(*cmd1);
                exit(1);
        }
    }
}

int runcommand(char** cline, int where, int narg, struct tok_types types) {
    pid_t pid;
    int status;
    if (!strcmp(*cline, "cd")) {
        if (narg > 2) {
            printf("cd: too many arguments\n");
            return -1;
        }
        run_cd(cline, narg);
        return 0;
    }
    if (!strcmp(*cline, "exit")) exit(0);
    switch (pid = fork()) {
        case -1:
            perror("smallsh");
            return -1;
        case 0:
            if (where == BACKGROUND) setpgid(getpid(), 0);
            else {
                act.sa_handler = SIG_DFL;
                sigaction(SIGINT, &act, NULL);
            }
            if (types.type2 == REDIRECTION)
                cline = run_redirection(cline, narg);
            if (types.type2 == PIPE) {
                run_pipe(cline, narg);
                perror("pipe run");
                exit(0);
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
