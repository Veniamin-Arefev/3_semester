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

    setbuf(stdout, NULL);

    int semid = semget(123456789, 5, 0600 | IPC_CREAT | IPC_PRIVATE);
    int shmid = shmget(123456789, sizeof(unsigned long long), 0600 | IPC_CREAT | IPC_PRIVATE);
    unsigned long long *pmem = shmat(shmid, NULL, 0);
    for (int i = 1; i <= 3; i++) {
        if (fork() == 0) {
            struct sembuf opsembuf;
            opsembuf.sem_num = 0;
            opsembuf.sem_op = 1;
            opsembuf.sem_flg = 0;
            struct sembuf mysembuf;
            mysembuf.sem_num = i;
            mysembuf.sem_op = -1;
            mysembuf.sem_flg = 0;
            unsigned long long mask;
            if (i == 1) {
                mask = 0xFFFFFF;
            } else if (i == 2) {
                mask = 0xFFFF;
            } else {
                mask = 0xFF;
            }
            while (semop(semid, &mysembuf, 1) >= 0) {
                printf("%d %llx\n", i, (*pmem)&mask);
                semop(semid, &opsembuf, 1);
            }
            _exit(0);
        }
    }
    unsigned long long number;
    struct sembuf opsembuf;
    opsembuf.sem_num = 0;
    opsembuf.sem_op = 1;
    opsembuf.sem_flg = 0;
    semop(semid, &opsembuf, 1);
    opsembuf.sem_op = -1;
    while (scanf("%llo", &number) > 0) {
        semop(semid, &opsembuf, 1);
        *pmem = number;
        struct sembuf mysembuf;
        mysembuf.sem_op = 1;
        mysembuf.sem_flg = 0;
        if ((number & 0x80000000) == 0) { //A
            mysembuf.sem_num = 1;
        } else if ((number & 0x40000000) == 0) { //B
            mysembuf.sem_num = 2;
        } else { //C
            mysembuf.sem_num = 3;
        }
        semop(semid, &mysembuf, 1);
    }

    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);

    while (wait(NULL) > -1);

    return 0;
}
