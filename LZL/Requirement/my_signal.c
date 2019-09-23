#include<stdio.h>
#include<signal.h>

void handler_sigint(int signo)
{
    printf("hello world\n");
}

int main()
{
    signal(SIGINT,handler_sigint);
    while(1);
    return 0;
}