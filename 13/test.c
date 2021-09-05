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

    int nproc;
    sscanf(argv[1], "%d", &nproc);
    int key;
    sscanf(argv[2], "%d", &key);
    int maxval;
    sscanf(argv[3], "%d", &maxval);

    int semid = semget(key, nproc, 0600 | IPC_CREAT);
    int shmid = shmget(key, sizeof(int), 0600 | IPC_CREAT);
    int *pmem = shmat(shmid, NULL, 0);
    *pmem = 0;

    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
