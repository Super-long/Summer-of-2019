#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc,char **argv,char **environ)
{
    int fd[2];
    pid_t pid;
    int stat_val;

    if(argc<2)
    {
        printf("error parameter!\n");
        exit(1);
    }
    if(pipe(fd))
    {
        perror("error in pipe!\n");
        exit(1);
    }
    pid=fork();
    switch (pid)
    {
        case 0:
            sleep(1);
            close(fd[1]);
            close(0);
            dup(fd[0]);   //读端与输入缓冲区相连
            execve("ctrlpocess",(void*)argv,environ);
            exit(0);
            break;
        case -1:
            perror("error in fork!\n");
            exit(1);
        default:
            close(fd[0]);
            write(fd[1],argv[1],strlen(argv[1]));
            break;
    }
    printf("hello!\n");
    wait(&stat_val);
    return 0;
}