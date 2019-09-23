#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main()
{
    int fd;//文件描述符

    if((fd==open("test_create.c",O_CREAT|O_EXCL|O_TRUNC,S_IRUSR|S_IWUSR))==-1)
    {
        perror("open");
        exit(1);
    }else
    {
        printf("success for open test_create.c\n");
    }
    close(fd);
    return 0;
}