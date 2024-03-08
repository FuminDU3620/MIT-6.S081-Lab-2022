#include <kernel/types.h>
#include <user/user.h>
#include <kernel/fcntl.h>

/*
处理流程：
p = get a number from left neighbor
print p
loop:
    n = get a number from left neighbor
    if (p does not divide n)
        send n to right neighbor
*/
/*
想到了一个简单的素数筛方法，通过维护一个int数组判断该数是否被筛掉，其扮演的是共享内存的角色.
没有通过父子进程的通信实现,应该用通讯顺序来实现共享内存
*/
/*
int
main()
{
    
    // int i = 0, j = 0;
    // int p[2];
    // pipe(p);
    // int input[10] = {2,3,4,5,6,7,8,9,10,11};
    // for(i = 0; i < 10; i++) {
    //     if(input[i] != 0) {
    //         int pid = fork();
    //         if(pid != 0) {
    //             if(input[i] != 0) {
    //                 write(1, input[i], 4);
                    
    //                 for(j = i+1; j < 10; j++) {
    //                     if(input[j] % input[i] == 0) input[j] == 0;
    //                     else write(p[0], input[j], 4);
    //                 }
    //                 input[i] = 0;
    //                 close(p[0]);
    //             } 
    //         } else {
    //             read(p[1], input, 4);

    //         }
                
    //         close(p[0]);
    //     }
        
    
    // }
    
}
*/

/*
递归的调用创建子进程，当管道为空时结束程序
*/
void child(int *pip)
{
    int p = 0, pip1[2];
    close(pip[1]); // 子进程中只使用父进程传来通道的读端，写段被父进程占用
    if(read(pip[0], &p, sizeof(p)) == 0) {
        close(pip[0]);
        exit(0);
    }
    fprintf(1, "prime %d\n", p);
        
    pipe(pip1); // 创建子进程与孙子进程的通信管道

    if(fork() == 0) 
        child(pip1);
    else {
        int n = 0;
        close(pip1[0]);
        while(read(pip[0], &n, sizeof(n))) {
            if(n % p != 0) write(pip1[1], &n, sizeof(n));
        }
        close(pip1[1]);
        wait(0);
    }
    exit(0);
}

int
main()
{
    int pip[2];
    pipe(pip); // 定义在父进程中，实现与子进程间的通信
    
    if(fork() == 0) {
        child(pip);
    } else {
        int i;
        close(pip[0]);
        for(i = 2; i <= 35; i++) {
            write(pip[1], &i, sizeof(i));
        }
            
        close(pip[1]);
        wait(0); // 避免子进程成为孤儿进程
    }
    exit(0);
}