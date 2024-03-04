#include "kernel/types.h"
#include "user/user.h"

int
mian(int argc, char *argv[])
{
    int p[2], q[2];
    pipe(p);
    pipe(q);
    if(fork() == 0) {
        close(0);
        dup(p[0]);
    }

}