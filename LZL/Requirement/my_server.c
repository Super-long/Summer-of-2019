#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

#define SERV_POT  4507     //服务器端口
#define LISTENQ  12        //连接请求队列的最大长度

#define INVAILD   'n'      //用户信息无效
#define VAILD     'y'      //用户信息有效

#define USERNAME 0
#define PASSWORD 1

struct usernifo
{
    char name[32];
    char password[32];
};

struct usernifo user[]={
    {"linux","unix"},
    {"hello","4561321"},
    {"asddasd","5451213"},
    {"asdasxzx","798745"},
    {" "," "}
};

int find_name(char *name)
{
    if(name==NULL)
    {
        perror("name is a invaild pointer!\n");
        return -2;
    }
    for(int i=0;user[i].name[0]!=' ';i++)
    {
        if(!strcmp(name,user[i].name))
        return i;
    }
    return -1;
}

void send_data(int conn_fd,const char *string) //传入一个连接套接字和字符串数据 
{
    if(send(conn_fd,string,strlen(string),0)<0)
    {
        perror("send\n");
        exit(1);
    }
}

int main()
{
    int sock_fd,conn_fd;
    int optval;
    int flag_recv=USERNAME;
    int name_num;
    pid_t pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr,serv_addr;
    char recv_buf[128];
    size_t ret;


    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd < 0)
    {
        perror("socket");
        exit(1);
    }
    optval=1;            //通用套接字　　socket退出后可正常连接　待设置的套接字选项的值及其大小
    if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int))<0)
    {
        perror("setsocket\n");
        exit(1);
    }
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET;      //协议族 ipv4 tcp/ip
    serv_addr.sin_port=htons(SERV_POT);//服务器端口
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); //IP地址
    
    if(bind(sock_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))<0)
    {
        perror("bind\n");
        exit(1);
    }

    if(listen(sock_fd,LISTENQ)<0)
    {
        perror("listen\n");
        exit(1);
    }

    cli_len = sizeof(struct sockaddr_in);
    while(1)
    {
        conn_fd = accept(sock_fd,(struct sockaddr*)&cli_addr,&cli_len);//新建立一个套接字
        //返回一个客户端套接字　并把客户端信息存入cli_addr  linux默认阻塞
        if(conn_fd<0)
        {
            perror("accept\n");
            exit(1);
        }
        printf("accept a new client ! ip:%s\n",inet_ntoa(cli_addr.sin_addr));
        if((pid=fork())==0)
        {
            while(1)
            {
                if((ret=recv(conn_fd,recv_buf,sizeof(recv_buf),0))<0)
                {
                    perror("recv\n");
                    exit(1);
                }
                recv_buf[ret-1]='\0';
                if(flag_recv==USERNAME)
                {
                    name_num=find_name(recv_buf);
                    switch (name_num)
                    {
                        case -1:
                            send_data(conn_fd,"n\n");//换行符为结尾标记
                            break;                            
                        case -2:
                            exit(1);//用户名为空　错误
                        default:
                            send_data(conn_fd,"y\n");
                            flag_recv=PASSWORD;
                            break;
                    }
                }else if(flag_recv==PASSWORD)
                {
                    if(strcmp(user[name_num].password,recv_buf)==0)
                    {
                        send_data(conn_fd,"y\n");
                        send_data(conn_fd,"welcome!\n");
                        printf("%s login!\n",user[name_num].name);
                        break;
                    }
                }
            }
            close(conn_fd);
            close(sock_fd);
            exit(1);
        }else
        {
            close(conn_fd);
        }       
    }
    return 0;
}   