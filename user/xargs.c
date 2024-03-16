#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include <stddef.h>

int
main(int argc, char* argv[]) {
    char* arguments[MAXARG+1];
    char buff;
    int i = 1, j = 0;

    // memmove(arguments[0], argv[1], sizeof(argv[1]));
    arguments[0] = argv[1];

    while(read(0, &buff, sizeof(buff)) > 0) {
        if(buff != '\n' && buff != ' ') {
            arguments[i][j++] = buff;
        } else if (buff == ' ')
        {
            arguments[i++][j] = 0;
        } else if (buff == '\n')
        {
            arguments[i] = NULL;
        }
    }
    
    if(fork() == 0) {
        exec(argv[0], arguments);
        exit(1); // If exec returns, it means it failed
    } else {
        wait(0);
    }
    exit(0);
}