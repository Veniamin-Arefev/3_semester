#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

enum {
    N = 5,
    THINKING = 0,
    HUNGRY = 1,
    EATING = 2,
    MAX_WAIT_TIME = 10
};

struct sembuf mutex_up_buf = {0, 1, 0};
struct sembuf mutex_down_buf = {0, -1, 0};

struct sembuf upforks[2] = {{0, 1, 0},
                            {0, 1, 0}};
struct sembuf downforks[2] = {{0, -1, 0},
                              {0, -1, 0}};

unsigned int myid;
int mutex;
int semid;
int shmid;
int *shmem;

void exiting(void) {
    shmdt(shmem);
    printf("Philosopher %d exited dinner room :)\n", myid);
    exit(0);
}

void Take_Forks(void) {
    if (semop(semid, downforks, 2) == -1) {
        exiting();
    }
    if (semop(mutex, &mutex_down_buf, 1) == -1) {
        exiting();
    }
    shmem[myid] = EATING;
    if (semop(mutex, &mutex_up_buf, 1) == -1) {
        exiting();
    }
}

void Put_Forks(void) {
    if (semop(semid, upforks, 2) == -1) {
        exiting();
    }
    if (semop(mutex, &mutex_down_buf, 1) == -1) {
        exiting();
    }
    shmem[myid] = THINKING;
    if (semop(mutex, &mutex_up_buf, 1) == -1) {
        exiting();
    }
}

void mySignalHandler(int sig) {
    shmdt(shmem);
    exit(0);
}

int main(int argc, char *argv[]) {

    setbuf(stdout, NULL);

    signal(SIGINT, mySignalHandler);
    char *keyfile = "phil.key";
    if (1) {
        int fd = open(keyfile, O_RDONLY | O_CREAT, 0666);
        close(fd);
    }
    semid = semget(ftok(keyfile, 67), N, 0666);
    if (semid == -1) {
        fprintf(stderr, "Can't get semaphore id\n");
        exit(1);
    }

    mutex = semget(ftok(keyfile, 69), 1, 0666);
    if (mutex == -1) {
        fprintf(stderr, "Can't get semaphore id\n");
        exit(1);
    }

    shmid = shmget(ftok(keyfile, 69), N * sizeof(int), 0666);
    if (shmid == -1) {
        fprintf(stderr, "Can't get shared memory id\n");
        exit(1);
    }
    shmem = shmat(shmid, NULL, 0);

    sscanf(argv[1], "%u", &myid);
    srand(myid);

    downforks[0].sem_num = (myid) % N;
    downforks[1].sem_num = (myid + 1) % N;
    upforks[0].sem_num = (myid) % N;
    upforks[1].sem_num = (myid + 1) % N;

    printf("Philosopher %d entered dinner room <3\n", myid);

    while (1) {
//        Think(i);
        sleep(rand() % MAX_WAIT_TIME+1);
        if (semop(mutex, &mutex_down_buf, 1) == -1) {
            exiting();
        }
        shmem[myid] = HUNGRY;
        if (semop(mutex, &mutex_up_buf, 1) == -1) {
            exiting();
        }
        Take_Forks();
//        Eat(i);
        sleep(rand() % MAX_WAIT_TIME+1);
        Put_Forks();
    }
    return 0;
}