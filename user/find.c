#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define MAX_PATH 512

/*
Initial thought seems there was confusion about how to 
correctly implement recursion, 
especially in avoiding unnecessary openings of directories and files 
that are not being directly searched for the target file name.
*/
void
find(char* path, char *name)
{
    char buf[MAX_PATH], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0) {
        fprintf(2, "ERROR: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ERROR: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(st.type != T_DIR) {
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0) {
            fprintf(2, "ERROR: cannot stat %s\n", buf);
            close(fd);
            continue;
        }
        
        if(st.type == T_FILE && strcmp(de.name, name) == 0) {
            printf("%s\n", buf);
        } else if(st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
            find(buf, name); // Recurse at new path
        }
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}