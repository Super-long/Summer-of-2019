#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

void my_err(const char * err_string ,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}

int lock_set(int fd,struct flock *lock)
{
    if(fcntl(fd,F_SETLK,lock)==0){
        if(lock->l_type==F_RDLCK)
        printf("set read lock : %d\n",getpid());
        if(lock->l_type==F_WRLCK)
        printf("set write lock :%d\n",getpid());
        if(lock->l_type==F_UNLCK)
        printf("set unlock : %d\n",getpid());
    }else
    {
        printf("operation failture!\n");
        return -1;
    } 
}

int test_lock(int fd,struct flock *lock)
{
    if(fcntl(fd,F_GETLK,lock)==0)
    {
        if(lock->l_type==F_UNLCK){
            printf("lock can be set!\n");
            return 1;  //成功
        }else{
            if(lock->l_type==F_RDLCK)
            printf("can't set read lock , ID:%d\n",lock->l_pid);
            if(lock->l_type==F_WRLCK)
            printf("can't set write lock  ID:%d\n",lock->l_pid);
            return -2;  //失败
        }
    }else{
        perror("get incompatible locks fail!\n");//获取不兼容的锁失败
        return -1; //原因
    }
}

int main()
{
    int          fd;
    int          set;
    struct flock lock;
    char        read_buf[40];

    if((fd=open("example_65",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
    my_err("open",__LINE__);
    if(write(fd,"test lock",10)!=10)
    my_err(write,__LINE__);
    memset(&lock,0,sizeof(struct flock));
    lock.l_len=0;
    //lock.l_type=F_UNLCK;
    lock.l_whence=0;
    lock.l_start=SEEK_SET;

    lock.l_type=F_RDLCK;

    if(test_lock(fd,&lock)==1)
    {
        lock.l_type=F_RDLCK;
        lock_set(fd,&lock);
    }
    lseek(fd,0,SEEK_SET);
    if((set=read(fd,read_buf,10))<0)
    my_err("read",__LINE__);

    read_buf[10]='\0';
    printf("%s",read_buf);

    getchar();

    lock.l_type=F_WRLCK;
    if(test_lock(fd,&lock)==1)
    {
        lock.l_type=F_WRLCK;
        lock_set(fd,&lock);
    }

    lock.l_type=F_UNLCK;
    lock_set(fd,&lock);
    close(fd);
    return 1;
}