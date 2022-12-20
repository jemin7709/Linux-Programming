#include "msg_header.h"

int msgid;

void handler(int signo) {
    msgctl(msgid, IPC_RMID, NULL);
    exit(0);
}

int main() {
    key_t key;
    struct msg_entry msg;
    struct sigaction act;
    int msglen;

    key = ftok("msg_header.h", 1);
    msgid = msgget(key, IPC_CREAT | QPERM);
    sigfillset(&act.sa_mask);
    act.sa_handler = handler;
    sigaction(SIGINT, &act, NULL);

    if (msgid == -1) {
        perror("msgget");
        return 1;
    }
    while ((msglen = msgrcv(msgid, &msg, MAXLEN, -10, IPC_NOWAIT)) > 0) {
        printf("Received Message = %s\n", msg.mtext);
        sleep(1);
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}