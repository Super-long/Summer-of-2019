#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    pid=fork();
    switch (pid)
    {
        case 0:
            printf("child process is running !\n");
            break;
        case -1:
            printf("调用失败！\n");
            break;
        default:
            printf("父进程ok！\n");
            break;
    }
    return 0;
}