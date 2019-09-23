#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void my_err(const char *err_string , int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}

int main()
{
    int ret;
    int access_mode;
    int fd;

    if(fd=open("test_read.txt",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU)==-1)
    my_err("open",__LINE__);
    if((ret=fcntl(fd,F_SETFL,O_APPEND))<0)   //与等于-1有什么不一样
    my_err("fcntl",__LINE__);

    access_mode=ret & O_ACCMODE;
    if(access_mode==O_RDONLY)
    printf("只读\n");
    if(access_mode==O_WRONLY)
    printf("只写\n");
    if(access_mode==O_RDWR)
    printf("读写\n");

    if(ret&O_APPEND)
    printf("oppend\n");
    if(rte&O_)

}
