#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define FIFO_READ  "writefifo"
#define FIFO_WRITE "readfifo"
#define BUF_SIZE   1024

int main(void)
{
    int wfd,rfd;
    char buf[BUF_SIZE];
    int len;

    umask(0);
    if(mkfifo(FIFO_WRITE,__S_IFIFO|0666)==-1)
    {
        perror("error in mkfifo\n");
        //exit(1);
    }
    while((rfd=open(FIFO_READ,O_RDONLY))==-1)
    {
        sleep(1);
    }
    wfd=open(FIFO_WRITE,O_WRONLY);
    if(wfd==-1)
    {
        printf("open fifo_write is failture : %s\n",strerror(errno));
        exit(1);
    }
    while(1)
    {
        len=read(rfd,buf,BUF_SIZE);
        if(len>0)
        {
            buf[len]='\0';
            printf("Client: %s",buf);
        }
        printf("Server: ");
        fgets(buf,BUF_SIZE,stdin);
        buf[strlen(buf)-1]='\0';  //意义是什么
        if(!strncmp(buf,"quit",4))
        {
            close(wfd);
            unlink(FIFO_WRITE);  //删除文件的时候和remove相同 
            close(rfd);
            exit(0);
        }
        write(wfd,buf,strlen(buf));
    }
    return 0;
}