#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

enum {
    N = 5,
    THINKING = 0,
    HUNGRY = 1,
    EATING = 2
};

struct sembuf up_mutex = {0, 1, 0};
struct sembuf down_mutex = {0, -1, 0};

int mutex;
int semid;
int shmid;
int *shmem;

void mySignalHandler(int sig) {
    semctl(mutex, 0, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID, 0);
    shmdt(shmem);
    shmctl(shmid, IPC_RMID, 0);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, mySignalHandler);
    char *keyfile = "phil.key";
    if (1) {
        int fd = open(keyfile, O_RDONLY | O_CREAT, 0666);
        close(fd);
    }

    semid = semget(ftok(keyfile, 67), N, 0666 | IPC_CREAT);
    if (semid == -1) {
        fprintf(stderr, "Can't get semaphore id");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        semctl(semid, i, SETVAL, (int) 1);
    }

    mutex = semget(ftok(keyfile, 69), 1, 0666 | IPC_CREAT);
    if (mutex == -1) {
        fprintf(stderr, "Can't get semaphore id");
        exit(1);
    }
    semctl(mutex, 0, SETVAL, 1);

    shmid = shmget(ftok(keyfile, 69), N * sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "Can't get shared memory id");
        exit(1);
    }
    shmem = shmat(shmid, NULL, 0);
    for (int i = 0; i < N; i++) {
        shmem[i] = THINKING;
    }

    printf("Inited all IPC recourses. Start main loop:\n");
    while (1) {
        sleep(1);
        for (int i = 0; i < N; i++) {
            printf("Philosopher with id <%d> is %s now\n", i,
                   shmem[i] == THINKING ? "thinking" : shmem[i] == HUNGRY ? "hungry" : "eating");
        }
        printf("\n");
    }
}