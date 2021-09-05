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

enum {
    COUNT_INDEX = 1,
    KEY_INDEX = 2,
    NPROC_INDEX = 3,
    ITER_COUNT_INDEX = 4,
    SEED_START_INDEX = 5
};

int getRandomNum(int max) {
    double random = rand() / ((double) RAND_MAX + 1);
    return ((int) (random * max));

}

int
main(int argc, char const *argv[]) {

    setbuf(stdout, NULL);

    unsigned int count;
    sscanf(argv[COUNT_INDEX], "%u", &count);
    int key;
    sscanf(argv[KEY_INDEX], "%d", &key);
    unsigned int nproc;
    sscanf(argv[NPROC_INDEX], "%d", &nproc);
    int iter_count;
    sscanf(argv[ITER_COUNT_INDEX], "%d", &iter_count);

    int semid = semget(key, count + 1, 0600 | IPC_CREAT | IPC_PRIVATE);
    int shmid = shmget(key, count * sizeof(int), 0600 | IPC_CREAT | IPC_PRIVATE);
    int *mas = shmat(shmid, NULL, 0);
    for (int i = 0; i < count; i++) {
        scanf("%d", mas + i);
        semctl(semid, i, SETVAL, (int) 1);
    }
    for (int i = 0; i < nproc; i++) {
        if (fork() == 0) {
            unsigned int seed;
            sscanf(argv[SEED_START_INDEX + i], "%u", &seed);
            srand(seed);

            for (int j = 0; j < iter_count; j++) {
                int ind1 = getRandomNum(count);
                int ind2 = getRandomNum(count);
                int value = getRandomNum(10);

                if (ind1 != ind2) {
                    struct sembuf mysembuf[2];
                    mysembuf[0].sem_num = ind1;
                    mysembuf[0].sem_op = -1;
                    mysembuf[0].sem_flg = 0;
                    mysembuf[1].sem_num = ind2;
                    mysembuf[1].sem_op = -1;
                    mysembuf[1].sem_flg = 0;
                    semop(semid, mysembuf, 2);

                    operation(mas, ind1, ind2, value);

                    mysembuf[0].sem_num = ind1;
                    mysembuf[0].sem_op = 1;
                    mysembuf[0].sem_flg = 0;
                    mysembuf[1].sem_num = ind2;
                    mysembuf[1].sem_op = 1;
                    mysembuf[1].sem_flg = 0;
                    semop(semid, mysembuf, 2);
                }
            }
            _exit(0);
        }
    }

    while (wait(NULL) > -1);

    for (int i = 0; i < count; i++) {
        printf("%d\n", mas[i]);
    }

    shmdt(mas);
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

