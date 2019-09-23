#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

void my_err(const char * err_string , int line)
{
    fprintf(stderr,"line : %d",line);
    perror(err_string);
    exit(1);
}

int my_read(int fd)
{
    int len;
    int ret;
    int i;
    char read_buf[64];
    if(lseek(fd,0,SEEK_END)==-1)
    my_err("lseek",__LINE__);

    if((len=lseek(fd,0,SEEK_CUR))==-1)
    my_err("lseek",__LINE__);

    if(lseek(fd,0,SEEK_SET)==-1)
    my_err("lseek",__LINE__);

    printf("len = %d\n",len);
    //if((ret=read(fd,read_buf,len))<0)
    //my_err("read",__LINE__);
    //for(int i=0;i<len;i++)
    //printf("%c",read_buf[i]);
    //return ret;
}

int main()
{
    int fd;
    char write_buf[]="hello world!";
    if((fd=creat("test_read.txt",S_IRWXU))==-1)  //两种方法打开不同
    if((fd=open("test_read.txt",O_RDWR|O_CREAT|O_TRUNC|O_RDONLY|O_WRONLY,S_IRWXU))==-1)
    {
        my_err("open",__LINE__);
    }else
    {
        printf("打开文件成功！\n");
    }

    if(write(fd,write_buf,strlen(write_buf))!=strlen(write_buf))
    {
        my_err("write",__LINE__);
    }else
    {
        printf("读入文件成功！\n");
    }
    
    my_read(fd);

    if(lseek(fd,10,SEEK_END)==-1)
    my_err("lseek",__LINE__);

    if(write(fd,write_buf,strlen(write_buf))!=strlen(write_buf))
    my_err("write",__LINE__);

    my_read(fd);

    close(fd);
    return 0;
}