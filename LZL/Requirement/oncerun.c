#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void th()
{
    printf("hello \n");
}

void pthread1()
{
    pthread_t pth=pthread_self();
    printf("Current pthread: %u\n",pth); //注意%u
    pthread_once(&once,th);  //使th函数在此进程中只运行一次
    printf("pthread 1 is end\n");
}

void pthread2()
{
    pthread_t pth=pthread_self();
    printf("Current pthread: %u\n",pth);
    pthread_once(&once,th);
    printf("pthread2 is end!\n");
}

int main()
{
    pthread_t thread1,thread2;
    pthread_create(&thread1,NULL,pthread1,NULL);
    pthread_create(&thread2,NULL,pthread2,NULL);    
    sleep(3);
    printf("main pthread is end!\n");
    exit(1);
}