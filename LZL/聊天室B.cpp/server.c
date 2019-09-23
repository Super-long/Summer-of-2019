#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h> 
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<mysql/mysql.h>
#include"solve.h" 

int fffffff;
int main()
{
    List_Init(status_per,node_status_t);
    MYSQL mysql; 
    mysql_init(&mysql);  //初始化一个句柄  
    mysql_library_init(0,NULL,NULL);//初始化数据库
    mysql_real_connect(&mysql,"127.0.0.1","root","lzl213260C","Login_Data",0,NULL,0);//连接数据库
    mysql_set_character_set(&mysql,"utf8");//调整为中文字符
    signal(SIGPIPE,SIG_IGN);   //ctrl+c stop  
    int sock_fd,conn_fd;
    int optval;
    int flag_recv=USERNAME;
    int name_num;
    pid_t pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr,serv_addr;
    size_t ret;
    int connect_size=0;      //目前连接数 
    pthread_t pth1;
    recv_t recv_buf;

    int epfd,nfds;
    struct epoll_event ev,events[EVENTS_MAX_SIZE];
    sock_fd=socket(AF_INET,SOCK_STREAM,0);  //服务器套接字
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
    setsockopt(sock_fd,SOL_SOCKET,SO_KEEPALIVE,(void*)&optval,sizeof(int));
    //从accept中返回的套接字继承套接字选项
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

    epfd=epoll_create(1);
    ev.data.fd= sock_fd;
    ev.events =EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP ;     //设置为监听读的状态
    //使用默认的LT模式 // epoll  事件只触发一次
    epoll_ctl(epfd,EPOLL_CTL_ADD,sock_fd,&ev);
    connect_size++;
    for(;;)
    {   
        nfds = epoll_wait(epfd,events,EVENTS_MAX_SIZE,-1);//等待可写事件
        for(int i=0;i<nfds;i++)
        {
            //printf(" The event type is %d\n",events[i].events);
            connect_size++;
            if(events[i].data.fd==sock_fd)       //服务器套接字接收到一个连接请求
            {
                if(connect_size>MAX_CONTECT_SIZE)
                {
                    perror("到达最大连接数!\n");
                    continue;
                }

                conn_fd=accept(events[i].data.fd,(struct sokcaddr*)&cli_addr,&cli_len);
                //网络字节序转换成字符串输出

                printf("%d  accept a new client ! ip:%s\n",++fffffff,inet_ntoa(cli_addr.sin_addr));

                if(conn_fd<=0)
                {
                    perror("error in accept\n");
                    printf("%s\n",strerror(errno));
                    continue;
                }
                ev.data.fd= conn_fd;
                ev.events =EPOLLIN | EPOLLONESHOT | EPOLLRDHUP;//|EPOLLOUT;  //设置事件可写与可写
                epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ev); //新增服务器套接字
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //客户端被挂掉　值设置为24 不把事件类型设置为SO_REUSEADDR　会发送很多可读事件
                //不清楚为什么　有点烦
                epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,0);    
                close(events[i].data.fd);
            }
            else if(events[i].events & EPOLLIN )  //接收到可读 且不是服务器套接字　不用判断　上面已判断 
            {
                if((ret=recv(events[i].data.fd,&recv_buf,sizeof(recv_buf),MSG_WAITALL))<0) //接收
                //包的格式已经提前制定好
                {
                    perror("recv\n");
                    continue;
                }
                if(!ret)//防止客户端异常退出时无法改变状态 客户端异常时会先发送一个大小为零的包
                {       //对端关闭会发送一个可读事件　但包的大小为一
                    //这个处理限定了一个ＩＰ只能登录一个账号
                    list_status_t curps;
                    char buf_tmp[256];
                    List_ForEach(status_per,curps){
                        //printf("一次n");
                        if(curps->fdd==events[i].data.fd){
                            sprintf(buf_tmp,"update Data set status = '0' where Account = '%s'",curps->account);
                            //printf("%s\n",buf_tmp);
                            mysql_query(&mysql,buf_tmp); //改变登录状态
                            List_DelNode(curps); //不正常退出修改状态信息
                            break;
                        }
                    }
                    char buf[128];
                    printf("The client with IP %d is disconnected\n",events[i].data.fd);
                    sprintf(buf,"select *from Data where send_recv_fd = %d",events[i].data.fd);
                    mysql_query(&mysql,buf);
                    MYSQL_RES *result = mysql_store_result(&mysql);
                    MYSQL_ROW row=mysql_fetch_row(result);
                    mysql_free_result(result);
                    sprintf(buf,"update Data set status = \"0\" where send_recv_fd = \"%d\"",events[i].data.fd);
                    mysql_query(&mysql,buf);
                    mysql_free_result(result);
                    continue;  
                }

                if(recv_buf.type==LOGIN)
                {
                    list_status_t tmp=(list_status_t)malloc(sizeof(node_friend_t));
                    tmp->fdd=events[i].data.fd;//发送者套接字
                    //printf("套接字%d\n",events[i].data.fd);
                    strcpy(tmp->account,recv_buf.send_Account); //连接者账号　//用来在删除时找到账号修改状态
                    List_AddTail(status_per,tmp); //加入在线者链表

                }
                recv_buf.send_fd = events[i].data.fd; //发送者的套接字已经改变 应转换为accept后的套接字
                recv_t *temp=(recv_t*)malloc(sizeof(recv_t)); //防止多线程访问一个结构体
                *temp=recv_buf;
                temp->epfd=epfd;
                temp->conn_fd=events[i].data.fd;
                //printf("进入线程\n");
                pth1=pthread_create(&pth1,NULL,solve,temp);//开一个线程去判断任务类型从而执行 值传递
                //solve((void*)temp);
            }  
        }
    }
    close(sock_fd);
    //还要关闭其他打开的套接字　别忘了
}
