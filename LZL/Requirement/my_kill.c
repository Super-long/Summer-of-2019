#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>

int main(int argc,char **argv)
{
    int signum=SIGTERM;
    pid_t pid;

    if(argc!=2 && argc!=4)
    {
        printf("my_kill error!\n");
        exit(1);
    }
    int i;
    for(i=0;i<argc;i++)
    {
        if(!strcmp(argv[i],"-s"))
        {
            signum=atoi(argv[i+1]);
            break;
        }
    }
    pid=atoi(argv[i+2]);
    if(kill(pid,signum)==-1)
    {
        perror("kill");
        exit(1);
    }
    return 0;
}