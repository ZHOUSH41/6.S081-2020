#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_LEN 100

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs cmd\n");
        exit(1);
    }
    char *cmd = argv[1];
    char buf;
    char paramv[MAXARG][MAX_LEN]; // args
    char *m[MAXARG];
    while (1)
    {
        int count = argc - 1;
        memset(paramv, 0, MAXARG*MAX_LEN);
        // copy the para of command para
        for (int i = 1; i < argc; i++) {
            strcpy(paramv[i-1], argv[i]);
        }

        int cursor = 0;
        int flag = 0;
        int read_result;
        while ((read_result = read(0, &buf, 1)) > 0 && buf != '\n'){
            if (buf == ' ' && flag == 1) {
                count++;
                cursor = 0;
                flag = 0;
            } else if (buf != ' ') {
                paramv[count][cursor++] = buf;
                flag = 1;
            }
        }
        if (read_result <= 0) break;

        for (int i = 0; i < MAXARG-1; i++) m[i] = paramv[i];
        m[MAXARG - 1] = 0;
        if (fork() == 0) {
            exec(cmd, m);
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}