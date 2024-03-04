#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(2, "sleep: one argument needed to be given!\n");
        exit(1);
    } else {
        fprintf(1, "(nothing happens for a little while)\n");
        sleep(atoi(argv[1]));
        exit(0);
    }
}