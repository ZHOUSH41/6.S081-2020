#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fd[2], fd_child[2];
    pipe(fd);
    pipe(fd_child);
    char buf[10];
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error\n");
        exit(1);
    } else if (pid == 0) {
        read(fd[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        write(fd_child[1], "pong", sizeof(buf));
    }
    else
    {
        write(fd[1], "ping", sizeof(buf));
        read(fd_child[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);
}