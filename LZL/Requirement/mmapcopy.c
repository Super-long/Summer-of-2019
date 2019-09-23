//这个程序是把test文件中的数据通过mmap函数映射到虚拟内存上
#include<unistd.h>
#include<sys/mman.h>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
int main(int argc,char ** argv)
{
    struct stat stat;
    int fd;
    fd=open("test",O_RDONLY,0);
    fstat(fd,&stat);
    char *hell;
    hell=mmap(NULL,stat.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    write(1,hell,stat.st_size);
    return 1;
}