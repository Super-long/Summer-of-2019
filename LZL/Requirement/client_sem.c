#include<sys/types.h>
#include<linux/sem.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
    key_t key;
    int semid,semval;
    union semun semopts;
    if((key=ftok(".",'s'))==-1)//生成一个键值
    {
        perror("error in ftok\n");
        exit(1);
    }
    if((semid=(semget(key,1,IPC_CREAT|0666)))==-1)
    {
        perror("error in semget\n");
        exit(1);
    }
    while(1)
    {
        if((semval=semctl(semid,0,GETVAL,0))==-1) //第一遍没有加括号 导致值全部是零
        {
            perror("error in semctl");
            exit(1);
        }
        if(semval>0)
        {
            printf("still %d can be used!\n",semval);
        }else
        {
            printf("no!\n");
            break;
        }
        sleep(3);
    }
    exit(1);
}