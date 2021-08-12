#include "kernel/types.h"
#include "user/user.h"

void cult(int *fd) {
    int n, pr[2];
    close(fd[1]);

    if (read(fd[0], &n, sizeof(int)) == 0) {
        exit(0);
    }
    pipe(pr);
    if (fork() == 0) {
        cult(pr);
    }
    else
    {
        close(pr[0]);
        printf("prime %d\n", n);
        int prime = n;
        while (read(fd[0], &n, sizeof(int))) {
            if (n % prime != 0) {
                write(pr[1], &n, sizeof(int));
            }
        }
        close(pr[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        cult(fd);
    } else {
        close(fd[0]);
        for (int i = 2; i < 36; i++)
        {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}