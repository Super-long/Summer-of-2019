#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void assisthread()
{
    printf("进入线程\n");
    sleep(3);
    pthread_exit(0);
}

int main()
{
    pthread_t assistthid;
    int status;

    pthread_create(&assistthid,NULL,(void *)assisthread,NULL);
    pthread_join(assistthid,(void*)&status);
    printf("%d 号线程已经死亡\n");
}