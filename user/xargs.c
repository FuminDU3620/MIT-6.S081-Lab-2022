#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    // printf("argc: %d\n", argc);
    char buf[512]; // Buffer to store input lines
    char *arguments[MAXARG];
    int n;

    // Prepare the initial part of the command to be executed
    for (int i = 1; i < argc && i < MAXARG; i++) {
        arguments[i - 1] = argv[i];
    }

    while ((n = read(0, buf, sizeof(buf))) > 0) {
        // The standard input read ends with a newline character and is read one line at a time.
        // printf("n: %d\n", n);
        // Pointer to the start of the next argument
        char *p = buf;
        for (int i = 0; i < n; i++) {
            // Split arguments and handle new lines
            if (buf[i] == '\n' || i == n - 1) {
                if (i == n - 1 && buf[i] != '\n') { // Handle case without newline at the end
                    i++;
                }
                buf[i] = 0; // Replace newline with null terminator to end the string

                // The next argument is the line read from input
                arguments[argc - 1] = p;

                // The argument list must be null-terminated
                arguments[argc] = 0;

                // Fork a child process
                int pid = fork();
                if (pid == 0) {
                    // Child process: execute the command
                    exec(arguments[0], arguments);
                    // If exec returns, it failed
                    fprintf(2, "exec failed\n");
                    exit(1);
                } else if (pid > 0) {
                    // Parent process: wait for the child to finish
                    wait(0);
                } else {
                    // Fork failed
                    fprintf(2, "fork failed\n");
                    exit(1);
                }

                // Prepare for the next input line
                p = buf + i + 1;
            }
        }
    }
    if (n < 0) {
        fprintf(2, "read error\n");
        exit(1);
    }

    exit(0);
}
