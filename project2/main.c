#include "smallsh.h"

static char cwd[255];
static struct passwd* pwd;

int main(int argc, char* argv[]) {
    int status;
    char* prompt = get_dir(getcwd(cwd, 255));
    act.sa_handler = int_handler;
    sigsetjmp(position, 1);
    while (sigaction(SIGINT, &act, NULL), userin(prompt) != EOF) {
        waitpid(-1, &status, 0 | WNOHANG);
        procline();
        prompt = get_dir(getcwd(cwd, 255));
    }
    return 0;
}
