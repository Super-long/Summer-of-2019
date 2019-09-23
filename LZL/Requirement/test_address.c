#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>

void display_time(const char *string)
{
    int seconds;
    seconds=time((time_t*)NULL);
    printf("%s %d\n",string,time);
}

int main()
{
    fd_set readfs;
    struct timeval timeout;
    int ret;
    FD_ZERO(&readfs); //清空描述符集合
    FD_SET(0,&readfs);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    while(1)    
    {
        timeout.tv_sec = 10;
        timeout.tv_usec = 0; 
        display_time("before select\n");
        ret=select(1,&readfs,NULL,NULL,&timeout);
        display_time("after select\n");
        
        switch(ret)
        {
            case 0:
                printf("no data in 10 seconds\n");
                exit(0);
                break;
            case -1:
                perror("select\n");
                exit(1);
                break;
            default:
                getchar();
                getchar();
                printf("data is avaliable now\n");
                break;
        }
    }
}
