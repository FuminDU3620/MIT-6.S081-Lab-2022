#include "kernel/types.h"
#include "user/user.h"

int
mian(int argc, char *argv[])
{
    int p[2], q[2];
    char *buf[8];
    pipe(p);
    pipe(q);
    if(fork() == 0) {
        write(p[0], "ping", 1);
        read(q[1], buf, 1);
        fprintf(1, "%d:  received pong", getpid());
        exit(0);
    } else if (fork() > 0)
    {
        read(p[1], buf, sizeof(buf));
        fprintf(1, "%d:  received ping", getpid());
        write(p[0], "pong", 1);
        exit(0);
    } else {
        fprintf(1, "ERROR: fork feiled!");
    }
}