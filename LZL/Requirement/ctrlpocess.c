#include<stdio.h>
#include<unistd.h>

int main(int argc,char ** argv)
{
    int n;
    char buf[1024];
    //while(1){
        if((n=read(stdin,buf,30))>0)  //输出-1 运行不正确
        {

            buf[n]='\0';
            printf("%s\n",buf);
            if(!strcmp(buf,"exit"))
            exit(0);
            if(!strcmp(buf,"getpid"))
            {
                printf("my pid : %d\n",getpid());
                sleep(3);
                exit(0);
            }
        }
        printf("%d",n);
    //}
    return 0;
}