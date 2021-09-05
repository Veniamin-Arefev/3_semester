#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct Msgbuf
{
    long msgtype;
    unsigned long long msgtext[2];
};

int
main(int argc, char const *argv[])
{
    int exit_code = 0;

    setbuf(stdout, NULL);

    int key;
    sscanf(argv[1], "%d", &key);
    unsigned int nproc;
    sscanf(argv[2], "%d", &nproc);
    long long value1;
    sscanf(argv[3], "%lld", &value1);
    long long value2;
    sscanf(argv[4], "%lld", &value2);
    unsigned long long maxval = 0;
    if (1) {
        long long temp;
        sscanf(argv[5], "%lld", &temp);
        maxval = llabs(temp);
    }

    int msgid = msgget(key, 0600 | IPC_CREAT | IPC_PRIVATE);
    int pid;
    for (int i = 0; i < nproc; i++) {
        if ((pid = fork()) == 0) {
            struct Msgbuf mybuf[1];
            // message count == process id + 1, because 0 process shouldnt use 0 id for msg
            while (msgrcv(msgid, mybuf, sizeof(mybuf->msgtext), i + 1, 0) >= 0) {
                unsigned long long temp = mybuf[0].msgtext[0] + mybuf[0].msgtext[1];
                printf("%d %llu\n", i, temp);
                if (temp > maxval) {
                    msgctl(msgid, IPC_RMID, NULL);
                    break;
                }
                struct Msgbuf newbuf[1];
                newbuf[0].msgtype = (long) ((temp % nproc) + 1);
                newbuf[0].msgtext[0] = mybuf[0].msgtext[1];
                newbuf[0].msgtext[1] = temp;
                msgsnd(msgid, newbuf, sizeof(mybuf->msgtext), 0);
            }
            _exit(0);
        } else {
            if (pid == -1) {
                msgctl(msgid, IPC_RMID, NULL);
                exit_code = 1;
            }
        }
    }
    struct Msgbuf mybuf[1];
    mybuf[0].msgtype = 1;
    mybuf[0].msgtext[0] = value1;
    mybuf[0].msgtext[1] = value2;
    msgsnd(msgid, mybuf, sizeof(mybuf->msgtext), 0);

    while (wait(NULL) > -1);

    return exit_code;
}
