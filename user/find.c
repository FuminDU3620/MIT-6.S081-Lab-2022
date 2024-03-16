#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define MAX_PATH 512

void
find(char* path, char *name)
{
    char buf[512], *p;
    int fd, fdc;
    struct dirent de;
    struct stat st, stc;

    if((fd = open(path, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

  switch (st.type)
  {
//   case T_FILE:
//     if(strcmp(fmtname(path), name))
//         printf("%s\n", path);
//     break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;
        if(strcmp(de.name, ".") == 1 || strcmp(de.name, "..") == 1)
            continue;

        if(strcmp(de.name, name) == 1) {
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            printf("%s\n", buf);
            break;
        }

        if((fdc = open(buf, 0) < 0)) {
            fprintf(2, "ls: cannot open %s\n", buf);
            return;
        }

        if(fstat(fdc, &stc) < 0){
            fprintf(2, "ls: cannot stat %s\n", path);
            close(fdc);
            return;
        }

        switch(stc.type)
        {
        case T_DIR:
            /* code */
            *p = 0;
            find(buf, name);
            close(fdc);
            break;
        case T_FILE:
            close(fdc);
            break;
        }
    }
    break;
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

