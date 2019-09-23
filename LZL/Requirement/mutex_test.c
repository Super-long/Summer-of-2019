#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int global_number;

void write_global()
{
    pthread_mutex_lock(&mutex);
    global_number++;
    sleep(3);
    pthread_mutex_unlock(&mutex);
}

void read_global()
{
    pthread_mutex_lock(&mutex);
    int x=global_number;
    pthread_mutex_unlock(&mutex);
    sleep(2);
    printf("%d\n",x);
}

int main()
{
    pthread_mutex_init(&mutex,NULL);  //初始化一个普通锁
    pthread_t pth1,pth2;
    pthread_create(&pth1,NULL,write_global,NULL);
    pthread_create(&pth2,NULL,read_global,NULL);
    printf("%d\n",global_number);
}