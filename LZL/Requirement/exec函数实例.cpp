#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>

int main(int argc,char **argv ,char ** environ)
{
    pid_t pid;
    int stat_val;
    pid = fork();
    switch (pid)
    {
        case -1:
            printf("failture!\n");
            break;
        case  0 :
            printf("this is child process !\n");
            printf("pid : %d,  parent pid : %d\n",getpid(),getppid());
            printf("uid : %d,  gid : %d\n",getuid(),getgid());
            execve("替换进程映像",argv,environ);
            printf("hello world\n");
            //exit(0);   
        default:
        printf("parent process is runing!\n");
            break;
    }
    wait(&stat_val);
    //exit(1);
}