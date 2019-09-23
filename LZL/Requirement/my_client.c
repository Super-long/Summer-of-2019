#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

#define INVAILD_USERINFO 'n'//用户信息无效
#define VAILD_USERINFO 'y'//用户信息有效

int my_recv(int conn_fd,char *data_buf,int len)
{
    static char recv_buf[BUFSIZ];  //8192
    static char *phread;
    static int len_remain = 0;
    int i;
    if(len_remain<=0) //能够第二次接着发　保存第一次没发完的数据
    {
        if((len_remain=recv(conn_fd,recv_buf,sizeof(recv_buf),0))<0)
        {
            perror("recv\n");
            exit(1);
        }else if(len_remain==0){
            return 0;
        }
        phread=recv_buf;
    }
    for(i=0;*phread!='\n';i++)      //防止一次发送没有发送完　所以设置为static 
    {
        if(i>len) return 0;
        data_buf[i]=*phread;
        phread++;
        len_remain--;
    }
    len_remain--;    //回车结束符号
    phread++;        //为了与上面进行对应
    return i;
}

int get_userinfo(char *buf,int len)
{
    int i,c;
    if(buf==NULL)
    return -1;
    i=0;
    while((c=getchar())!='\n' && c!=EOF && i<=len-2)
    {
        buf[i++]=c;
    }
    buf[i++]='\n';
    buf[i++]='\0';
    return 0;
}

void input_userinfo(int conn_fd,const char *string)
{
    char input_buf[32];
    char recv_buf[BUFSIZ];
    int flag_userinfo;
    do{
        printf("%s:",string);
        if(get_userinfo(input_buf,32)<0)
        {
            perror("get_userinfo\n");
            exit(1);
        }
        if(send(conn_fd,input_buf,strlen(input_buf),0)<0)
        {
            perror("send\n");
            exit(1);
        }
        if(my_recv(conn_fd,recv_buf,strlen(recv_buf))<0)
        {
            perror("my_recv\n");
            exit(1);
        }
        if(recv_buf[0]==VAILD_USERINFO)
        flag_userinfo=VAILD_USERINFO;
        else
        {
            printf("error! please enter again\n");
            flag_userinfo=INVAILD_USERINFO;
        }
    }while(flag_userinfo==INVAILD_USERINFO);
}

int main(int argc,char **argv)
{
    int i;
    int ret;
    int conn_fd;
    int serv_port;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];

    if(argc!=5)
    {
        printf("-p serv_port -a serv_address\n");
        exit(1);
    }
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET; //本机IP
    for(i=0;i<argc;i++)    //解析命令行
    {
        if(!strcmp(argv[i],"-p"))
        {
            serv_port=atoi(argv[i+1]);
            if(serv_port<0 || serv_port>65535)
            {
                perror("invaild port\n");
                exit(1);
            }else
            {
                serv_addr.sin_port=htons(serv_port);//主机的数字转化为网络字节序
            }
        }
        if(!strcmp(argv[i],"-a"))
        {
            if(inet_aton(argv[i+1],&serv_addr.sin_addr)==0)
            {
                perror("invaild server ip!\n");
                exit(1);
            }
        }
    }
    if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr==0) //unsigned long
    {
        perror("invaild param!\n");
        exit(1);
    }
    //ipv4 tcp/ip协议    tcp流套接字
    conn_fd=socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd<0)
    {
        perror("socket\n");
        exit(1);
    }
    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0)
    {
        perror("connect\n");
        exit(1);
    }

    input_userinfo(conn_fd,"username:");
    input_userinfo(conn_fd,"password:");

    if((ret=my_recv(conn_fd,buf,sizeof(buf)))<0)
    {
        perror("my_recv\n");
        exit(1);
    }
    for(i=0;i<ret;i++)
    {
        printf("%c",buf[i]);
    }
    putchar('\n');
    close(conn_fd);
}