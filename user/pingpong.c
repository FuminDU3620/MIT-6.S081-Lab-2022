#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid, status;
    pid = fork();
    int p[2], q[2];
    char *buf[8];
    pipe(p);
    pipe(q);

    if(pid == 0) {
        read(p[1], buf, sizeof(buf));
        fprintf(1, "%d: received ping\n", getpid());
        write(q[0], "pong", 1);
    } else {
        write(p[0], "ping", 1);
        wait(&status);
        if(status == -1)
            fprintf(2, "the child exit with status %d\n", status);
        read(q[1], buf, sizeof(buf));
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}