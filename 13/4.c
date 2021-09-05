#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>


int
main(int argc, char const *argv[]) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    int count;
    sscanf(argv[1], "%d", &count);
    int semid = semget(123456789, count + 2, 0600 | IPC_CREAT | IPC_PRIVATE);

    for (int i = 0; i < count; i++) {
        if (fork() == 0) {
            struct sembuf mysembuf;
            mysembuf.sem_num = i;
            mysembuf.sem_op = -1;
            mysembuf.sem_flg = 0;
            while (semop(semid, &mysembuf, 1) >= 0) {
                int number;
                if (scanf("%d", &number) > 0) {
                    printf("%d %d\n", i, number);
                    struct sembuf newsembuf;
                    int out = number % count;
                    out = (out + count) % count;
                    newsembuf.sem_num = out;
                    newsembuf.sem_op = 1;
                    newsembuf.sem_flg = 0;
                    semop(semid, &newsembuf, 1);

                } else {
                    semctl(semid, 0, IPC_RMID);
                    break;
                }
            }
            _exit(0);
        }
    }

    struct sembuf mysembuf;
    mysembuf.sem_num = 0;
    mysembuf.sem_op = 1;
    mysembuf.sem_flg = 0;
    semop(semid, &mysembuf, 1);

    while (wait(NULL) > -1);



    return 0;
}
