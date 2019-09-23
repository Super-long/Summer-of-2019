#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>
#include<pthread.h> 
#include<stdio.h>
#include"Data.h"
#include<map>
#include<stack>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<iostream>
using namespace std;
char gl_account[MAX_ACCOUNT]; //以一个全局变量记录账号 
int  fact_fd;          //记录服务器套接字
char fact_name[MAX_USERNAME];
int tt=1;  //更新映射表主键
int dd=1;  //更新群与好友和消息的映射关系

map<int,int>mp; //主键为账号　键值为头指针
map<int,int>mp_group;//群的映射表

list_friend_t head;//存储一次登录的好友信息

list_messages_t Messages[1024]; //与每一个好友的消息链表

list_messages_t Message_BOX;//消息盒子链表

list_group_t group_head; //群链表 //每一个群分配一个唯一主键　

list_member_t member[1024];//每一个群中的群员链表

list_group_messages_t group_messages[1024];//每一个群中的消息链表



int my_recv(int conn_fd,char *data_buf,int len) //这个可以参考　也很巧妙
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

//因为这里用了char* 所以所有的地方都要进行强转
int my_recv_tmp(int conn_fd,char *data_buf,int len)
{
        char *p = data_buf;
        //printf("     需要接收  %d\n",len);
        //memset(data_buf, 0, len);
        while (len > 0) {
                ssize_t n = recv(conn_fd, p, len, 0);
                if (n < 0)
                    perror("error in recv\n");
                else if (n == 0)
                    printf("接收到包大小为零\n");
                else {
                        //printf("recv %zd bytes: %s\n", n, p);
                        p += n;
                        len -= n;
                }
        }

        //因为tcp是流式的　一个包可能被分成几个小段来发送
        //所以必须要用一个循环来接收　保证包的大小
        //p[len]='\0';
        return len;
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
    buf[i++]='\0';
    return 0;
}

int input_userinfo(recv_t *temp)
{
    int flag_userinfo=0;
    char password[MAX_RECV];
    fflush(stdin);
    printf("account:");
    if(get_userinfo(gl_account,MAX_ACCOUNT)<0)
    {
        perror("get_userinfo\n");
        exit(1);
    }
    strcpy(temp->send_Account,gl_account);
    printf("password:");
    if(get_userinfo(password,MAX_PASSWORD)<0)
    {
        perror("get_userinfo\n");
        exit(1);
    }
    strcpy(temp->message,password);
}


//接包
//执行这个函数以后为在此文件中存储一个好友信息链表　可以进行各种操作
//计划把数据接收来以后进行存放　以链表形式存储　分页形式显示
//最后有一个标记位为　EOF　的结束包　
int FetchAll_for_Friend_List()
{
    //List_DelNode(head);
    recv_t pacage;
    int ans=0;
    while(1)
    {
        printf("好友列表　%d\n",sizeof(recv_t));
        //if(my_recv_tmp(fact_fd,(char*)&pacage,sizeof(recv_t))<0)
        //perror("error in recv\n");//收包
        int ret = recv(fact_fd,&pacage,sizeof(pacage),MSG_WAITALL);
       // pacage=(recv_t)pacage;
        //printf(":::::收到好友包 %s %d\n",pacage.message,pacage.type);

        if(pacage.type==EOF_OF_BOX)
        break;//接收到EOF结束符　退出接收循环

        //利用数据包中数据对链表结点进行赋值
        list_friend_t temp=(list_friend_t)malloc(sizeof(node_friend_t));
        bzero(temp,sizeof(node_friend_t));
        temp->status=pacage.conn_fd;//状态
        strcpy(temp->recv_account,pacage.message_tmp);//好友账号
        strcpy(temp->nickname,pacage.message);//昵称
        //printf("%s\n",temp->nickname);
        //printf("%d::%d\n",++ans,temp->status);
        List_AddTail(head,temp);//建立链表
    }
    //getchar();
}


int login_client(int conn_fd,char *username)
{
    map<int,int>pp;
    List_Init(Message_BOX,node_messages_t); //初始化消息盒子链表　后面只需要添加即可
    fact_fd=conn_fd; //给全局变量赋值
    int              ret;
    recv_t           Package;
    Package.type   = LOGIN;
    Package.send_fd= conn_fd;
    char buf[MAX_RECV];
    int number=3;
    getchar();
    system("clear");
    while(number--)  //三次机会
    {
        system("clear");
        input_userinfo(&Package);
        if(send(conn_fd,&Package,sizeof(recv_t),0)<0)//发送一个登录请求
        {
            perror("error in send\n");
            return 0;  //错误退出
        }
        //如果登录请求正确
        bzero(username,sizeof(username));
        if((ret=my_recv_tmp(conn_fd,username,MAX_USERNAME))<0) //默认阻塞
        {
            perror("error in my_recv\n");
            exit(1);
        }
        //printf("%s::\n",username);
        if(username[0]==ERROR_IN_LOGIN)
        {
            printf("account or password error!\n");
            continue;
        }else break;
    }
    strcpy(fact_name,username);//给全局变量赋值
    if(number==-1) return 0;
    //登录成功以后开始接收离线消息盒子里的消息
    my_recv_tmp(conn_fd,buf,14);
    Box_t tttt;
    bzero(&tttt,sizeof(Box_t));
    if(buf[0]=='@')
    {
        printf("消息盒子无记录！\n");//登陆成功 离线消息盒子无记录　进入服务界面
        //recv(conn_fd,&tttt,sizeof(Box_t),MSG_WAITALL);
    }
    else
    {
        Box_t box;
        char buffer[32];
        //printf("进行到循环\n");
        while(1)
        {
            if(my_recv_tmp(conn_fd,(char*)&box,sizeof(box))<0)
            perror("error in recv\n");
            if(box.type==EOF_OF_BOX)
            {
                //printf("%s %d %s \n",box.message,box.type,box.account);
                break;
            }
            //printf("%s:\n%s\n",box.account,box.message); //显示离线发送来的信息
            if(box.type==ADD_FRIENDS)
            {
                printf("\033[32m There is a friend request: \033[0m \n");
                printf("%s:%s\n",box.account,box.message);
                printf("YES   [Y]    NO    [N]\n");
                scanf("%s",buffer);
                getchar();
                Package.type=ADD_FRIENDS_QUERY;
                strcpy(Package.message,buffer);

                //有发送者和接收者就可以确定好友关系　加入数据库
                strcpy(Package.recv_Acount,box.account);//发送者
                strcpy(Package.send_Account,gl_account);//接收者　//一种新的事件　epoll来判断
                if(send(conn_fd,&Package,sizeof(recv_t),0)<0)
                perror("error in send friend request\n"); //根据首字母判断

                list_friend_t fri = (list_friend_t)malloc(sizeof(node_friend_t));
                strcpy(fri->send_account,Package.recv_Acount);
                List_AddTail(head,fri);
            }
            
            //说明这是一个消息类型的包　服务器会先发　你发送的消息　后发你接收的消息
            //把这些接收来的包放入账号主键唯一对应的键值
        }
    }
    //下面这行代码是当时调错用的　错误原因为服务器逻辑出现问题

    //Box_t box;
    //recv(conn_fd,&box,sizeof(box),0);   //处理多接收了一个包
    //printf("%s %d %s \n",box.message,box.type,box.account);
    //接收完离线消息盒子记录开始接收好友信息列表
    //实现为直接开始收包　最后一个结束包其中无数据　标记位为EOF
    FetchAll_for_Friend_List();
    //好友关系记录发送完成　结尾为一个结尾包　
    //printf("好友列表加载完成\n");
    //接收到一个标记包　表示是否有消息记录
    printf("是否有消息 %d\n",sizeof(buf));
    if(my_recv_tmp(conn_fd,buf,14)<0)
    perror("error in client recv messages record\n");
    int pause_tmp_for_messages=0;
    if(buf[0]=='@')
    {
        printf("消息记录无记录！\n");
        pause_tmp_for_messages=1;
    }
    else{
        //若有消息记录开始加载
        Box_t box;
        while(1)
        {
             //printf("开始接收消息　%d\n",sizeof(Box_t));
            if(my_recv_tmp(conn_fd,(char*)&box,sizeof(Box_t))<0)
            perror("error in recv\n");
/*             printf("%s\n",box.message);
            getchar(); */
            //printf("%s %s\n",box.message,box.account);
            if(box.type==EOF_OF_BOX) //接收到结束包会退出
            break;
            if(box.type==SEND_MESSAGES)//顺序为　链表前为老信息　链表后为新信息
            {
                if(!mp[atoi(box.account)])//表中值为零说明第一次
                {
                    mp[atoi(box.account)]=tt++; //对应唯一二维数组的索引
/*                     printf("%d %s %d\n",strlen(box.account),box.account,tt-1);
                    getchar(); */
                    List_Init(Messages[mp[atoi(box.account)]],node_messages_t);
                }
                list_messages_t temp=(list_messages_t)malloc(sizeof(node_messages_t)); //销毁链表
                strcpy(temp->messages,box.message);
                strcpy(temp->send_account,box.account);//好友账号
                strcpy(temp->nickname,box.usename); //username 必为好友本人的
                temp->type=box.type;
                List_AddTail(Messages[mp[atoi(box.account)]],temp);//将消息加入链表
            }
        }
    }
    if(pause_tmp_for_messages)
    {
        Box_t ttttt;
        bzero(&ttttt,sizeof(Box_t));
        recv(conn_fd,&ttttt,sizeof(Box_t),MSG_WAITALL);
    }

    //在这里开始接收群消息
    recv_t tmp;
   
    int yyy=0;
    while(1)
    {   
        //if(my_recv_tmp(conn_fd,(char*)&tmp,sizeof(recv_t))<0)
        bzero(&tmp,sizeof(recv_t)); //接收到的消息要加 入两个链表
        tmp.type = 1000;
        if(recv(conn_fd,&tmp,sizeof(recv_t),MSG_WAITALL)<0)
        perror("error in recv\n");
        printf("标记位　sd%s %d \n",tmp.message,tmp.type);
        if(tmp.type==EOF_OF_BOX)
        break;
        if(tmp.type==NULL_OF_GROUP)
        {
            yyy=1;
            break;
        }
        list_group_t cur=(list_group_t)malloc(sizeof(node_group_t));
        strcpy(cur->account,tmp.message);//群号
        strcpy(cur->nickname,tmp.message_tmp);//群昵称
        List_AddTail(group_head,cur);//加到群链表
    }

    if(!yyy) //这个代码写的很丑陋　
    //给每一个群添加消息信息     //这个地方其实设计的有问题　会将数据库所有的消息都发来　

    //这个接收包的过程可以对群消息和群成员的链表进行填充
    while(1){
        bzero(&tmp,sizeof(recv_t)); //清空结构体　防止隐性错误
        if(my_recv_tmp(conn_fd,(char*)&tmp,sizeof(recv_t))<0)
        perror("error in recv\n");
        if(tmp.type==EOF_OF_BOX)
        break; //接收到结束包
        if(mp_group[atoi(tmp.recv_Acount)]==0)
        {
            mp_group[atoi(tmp.recv_Acount)]=tt++;//分配唯一的映射关系
            List_Init(group_messages[mp_group[atoi(tmp.recv_Acount)]],node_group_messages_t)
            List_Init(member[mp_group[atoi(tmp.recv_Acount)]],node_member_t);
        }
        //分配群消息链表
        list_group_messages_t cur=(list_group_messages_t)malloc(sizeof(node_group_messages_t));
        bzero(cur,sizeof(node_group_messages_t));
        strcpy(cur->account,tmp.send_Account);//好友账号
        strcpy(cur->messages,tmp.message_tmp);//消息
        cur->type=tmp.type; //群成员的等级
        //printf("消息：%s\n",cur->messages);
        List_AddTail(group_messages[mp_group[atoi(tmp.recv_Acount)]],cur);

        //分配群成员链表
        //printf(" 成员                         %s\n",tmp.send_Account);
        
/*         if(pp[atoi(tmp.send_Account)]==0)  //有bug　一个成员只能在所有群中加载一次
        {
            pp[atoi(tmp.send_Account)]++;
            printf(" 成员                         :::%s\n",tmp.recv_Acount);
            list_member_t cdr=(list_member_t)malloc(sizeof(node_member_t));
            strcpy(cdr->account,tmp.send_Account);//其他消息并没有
            //printf("成员 %s\n",cdr->account);
            List_AddTail(member[mp_group[atoi(tmp.recv_Acount)]],cdr);
        } */

        //保证每一个群每个号只出现一次
        list_member_t cdr=(list_member_t)malloc(sizeof(node_member_t));
        int lzl=0;
        //printf("群账号　%s\n",tmp.recv_Acount);
        List_ForEach(member[mp_group[atoi(tmp.recv_Acount)]],cdr)
        {
            if(!strcmp(cdr->account,tmp.send_Account))
            {
                lzl=1;
                break;
            }
        }
        //原因在于cdr有malloc后分配的值　所以其值其实始终相当于消息链表的头结点　所以插入后一直没有值
        //其实最简单的就是不给cur分配内存空间
        //free(cdr);   //这里free会把原链表free
        if(!lzl)
        {
            list_member_t cudr=(list_member_t)malloc(sizeof(node_member_t));
            //这里插入不可以使用上面的cdr 刚开始没写free时也不可以　链表会乱
            strcpy(cudr->account,tmp.send_Account);//其他消息并没有
            List_AddTail(member[mp_group[atoi(tmp.recv_Acount)]],cudr);
        }
    }
    

    return 1;  //登陆成功　进入服务界面
}

int register_client(int conn_fd,char *account)  //注册请求　返回一个账号
{
    int ret;
    recv_t           Package;
    Package.type   = REGISTER;
    Package.send_fd= conn_fd;
    char password[MAX_PASSWORD];
    char telephone[MAX_TELEPHONE];
    char nickname[MAX_USERNAME];
    system("clear");

    //收集注册信息
    printf("Welcome to register account!\n");
    printf("please enter your password,we will give your a unique account.\n");
    printf("password:");
    getchar();
    get_userinfo(password,MAX_PASSWORD);
    printf("Get it back for your password\n");
    printf("telephone:");
    get_userinfo(telephone,MAX_TELEPHONE);
    printf("please enter you nickname!\n");
    get_userinfo(nickname,MAX_USERNAME);
    strcpy(Package.recv_Acount,nickname);
    strcpy(Package.message_tmp,telephone);
    strcpy(Package.message,password);

    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in register send\n");
        return 0;
    }
    if((ret=my_recv_tmp(conn_fd,account,MAX_ACCOUNT))<0)
    {
        perror("error in register send\n");
        return 0;
    }
    if(account[0]==ERROR_IN_LOGIN)
    {
        perror("error in server data\n");
        return 0;
    }
    printf("This is your Account ,Don't forget it!\n");
    printf("Account:%s\nPlease enter again!\n",account);
    printf("please enter enter key for quit!\n");
    getchar();
    return 1;   //发送正确且收到账号消息返回１
}

int Retrieve_client(int conn_fd)
{
    //变量声明
    int              ret=0;
    recv_t           Package;
    Package.type   = RETRIEVE;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    char telephone[MAX_TELEPHONE];
    char new_password[MAX_PASSWORD];
    char flag[32]; //仅用作收一次数据
    system("clear");
    //数据获取
    getchar();
    printf("Please enter your Account:");
    get_userinfo(Account,MAX_ACCOUNT);
    printf("Please enter your telephone:");
    get_userinfo(telephone,MAX_TELEPHONE);
    printf("enter your new password:");
    get_userinfo(new_password,MAX_PASSWORD);
    strcpy(Package.send_Account,Account);
    strcpy(Package.message_tmp,telephone);
    strcpy(Package.message,new_password);
    //发包
    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in retrieve send\n");
        return 0;
    }
    if((ret=my_recv_tmp(conn_fd,flag,32))<0)
    {
        perror("error in retrieve send\n");
        return 0;
    }
    if(flag[0]=='@')  //正确的话发送一个"y“
    {
        printf("Password change failed\n");
        return 0;
    }
    printf("Password changed successfully\n");
    return 1;
}

int Add_Friend(int conn_fd)
{
    int              ret=0;
    recv_t           Package;
    Package.type   = ADD_FRIENDS;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    char message[MAX_RECV];   //添加好友时给对方发送的话
    char temp[64];   //就是一个接收消息的缓冲区
    system("clear");
    getchar();
    printf("Please enter a Account you want to add:");
    get_userinfo(Account,MAX_ACCOUNT);
    printf("please enter your friendly greeting:\n");
    get_userinfo(message,MAX_RECV);
    strcpy(Package.recv_Acount,Account);
    strcpy(Package.message,message);
    strcpy(Package.send_Account,gl_account);  //全局变量

    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in add friend send\n");
        return 0;
    }
    printf("The message has been sent,please wait for it to be accepted\n");
    getchar();
    return 1;
}

int Del_Friend(int conn_fd)
{
    int              ret=0;
    recv_t           Package;
    Package.type   = DEL_FRIENDS;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    list_friend_t curpos;
    char message[MAX_RECV];   //添加好友时给对方发送的话
    char temp[64];   //就是一个接收消息的缓冲区
    system("clear");
    getchar();
    printf("Please enter a Account you want to delete:");
    get_userinfo(Account,MAX_ACCOUNT);
    printf("Do you sure delete %s?[Y]yes / [N]no\n",Account);
    scanf("%s",temp);
    if(!(temp[0]=='Y'||temp[0]=='y'?1:0)) return 0;
    strcpy(Package.recv_Acount,Account);
    strcpy(Package.send_Account,gl_account);  //全局变量　代表本身的账号
    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in del friend send\n");
        return 0;
    }
    List_ForEach(head,curpos)
    {
        if(!strcmp(Account,curpos->recv_account));
        {
            List_DelNode(curpos);
            break;
        }
    }
    printf("%s have been delete!\n"); //没检测是否存在
    getchar();
    return 1;
}


int send_friend_messages(char *account,char *Message)
{
    recv_t package;
    package.type=SEND_MESSAGES;
    strcpy(package.message,Message);         //消息
    strcpy(package.recv_Acount,account);     //消息接收者
    strcpy(package.send_Account,gl_account); //消息发送者
    if(send(fact_fd,&package,sizeof(recv_t),0)<0)
    perror("error in send friend messages\n");  //相当于只发不收 收在一个专门收的函数中
    return 0;
}



//这个函数的意义是打开一个窗口　你可以与你输入的账号的好友聊天
//除非点击输入　否则每隔0.5秒刷新一次页面　防止有新的信息收到而无法显示
//单独开的那个线程把所有收到的消息放到一个链表中　对于好友请求没有读就不会从数据库中删除
//每次刷新页面时在这个链表中搜索　有新的消息就打印出来　
//服务器如何存储消息信息　与好友请求同存储于一张表中　在每次登录时发送　对于每一个好友建立一个以其
//账号为主键的map　键值为指针　指向一个链表 //在每次搜寻和发送时都在链表中中加入消息　同时服务器进行存储　

int Chat(char *account)//参数为好友账号
{
    char Message[MAX_RECV];
    Pagination_t paging;
    node_messages_t *pos;
    char acc_tmp[MAX_ACCOUNT];
    strcpy(acc_tmp,account);
    int i;
    char choice;
    int flag=0;
    list_messages_t curos;
    paging.totalRecords=0;
    if(Messages[mp[atoi(account)]]==NULL) //说明没有消息　链表还未初始化
    {
        printf("初始化\n");
        mp[atoi(account)]=++tt;
        List_Init(Messages[mp[atoi(account)]],node_messages_t);
    }
    List_ForEach(Messages[mp[atoi(account)]],curos) paging.totalRecords++;
    //遍历消息链表
	paging.offset = paging.totalRecords;
	paging.pageSize = MESSADES_PAGE_SIZE;
            Paging_Locate_FirstPage(Messages[mp[atoi(account)]], paging);
            while(1){
    
            system("clear");
            //printf("链表长度：%d\n",paging.totalRecords);

            //在消息盒子中查找是否有正在发消息的好友发送来的消息
            List_ForEach(Message_BOX,curos) 
            {
                //消息肯定是发送者是正在聊天的好友的账号
                if(curos->type==SEND_MESSAGES && !strcmp(curos->send_account,account))
                {
                    list_messages_t temp = (list_messages_t)malloc(sizeof(node_messages_t));
                    strcpy(temp->messages,curos->messages);
                    strcpy(temp->recv_account,curos->recv_account);
                    strcpy(temp->nickname,curos->nickname);
                    List_AddTail(Messages[mp[atoi(account)]],temp);
                    paging.totalRecords+=1;//更新消息链表
                    List_FreeNode(curos); //这个消息已经载入消息链表　可以删除了
                }
            }
/*             List_ForEach(Messages[mp[atoi(account)]],curos)
            {
                cout << curos->messages << endl;
            } */

            printf("\001\033[1m\002");
            printf("\033[34m");
            printf(
                    "\n==================================================================\n");
            printf(
                    "**************************** %s ****************************\n",account);//有消息可以用这个　Messages[mp[account]]->nickname
                               //没有消息不就凉了
            printf(
                    "------------------------------------------------------------------\n");
                    //printf("((((%d,%s,%d\n",mp[atoi(acc_tmp)],acc_tmp,strlen(acc_tmp));
            printf("\001\033[0m\002");
            Paging_ViewPage_ForEach(Messages[mp[atoi(acc_tmp)]], paging, node_messages_t, pos, i){
                //链表中名称必为好友昵称
                //printf("%s :%s :\n",pos->nickname,fact_name);
                if(strcmp(pos->nickname,fact_name))//怎么比都可以
                {
                    printf("\033[32m %-65s \033[0m \n",pos->nickname);
                    printf("\033[32m %-65s \033[0m \n",pos->messages);
                }else{
                    printf("\033[35m %65s \033[0m \n",fact_name);
                    printf("\033[35m %65s \033[0m \n",pos->messages);
                }
                putchar('\n');
            }

            printf("\001\033[1m\002");
            printf("\033[34m");
            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[P]revPage | [N]extPage | [I]uput | [R]eturn");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            printf("\001\033[0m\002");
            fflush(stdin);
            scanf("%c", &choice);
            getchar();
            fflush(stdin);

            switch (choice) {
                case 'I':
                case 'i':
                {
                    //将消息在发向服务器的同时把消息存入　本好友账号映射的消息链表
                    printf("please enter:\n");
                    cin.getline(Message,sizeof(Message)); //发送者　接收者　消息　昵称不急
                    list_messages_t temp=(list_messages_t)malloc(sizeof(node_messages_t));
                    strcpy(temp->messages,Message);
                    strcpy(temp->send_account,gl_account);
                    strcpy(temp->nickname,fact_name);//随便即可　只要不和自己的名字重合
                    strcpy(temp->recv_account,account);
/*                     if(mp[account]==0)
                    mp[account]=++tt; */
                    List_AddTail(Messages[mp[atoi(account)]],temp);//加入到消息链表
                    paging.totalRecords++;
                    send_friend_messages(account,Message);
                }
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(Messages[mp[atoi(acc_tmp)]], paging, -1, node_messages_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(Messages[mp[atoi(acc_tmp)]], paging, 1, node_messages_t);
                    }
                    break;
                case 'r':
                case 'R':
                    flag=1;
                    break;
            }
            if(flag) break;
        } 
}



int show_friend_list()//套接字为全局变量
{
    char account[MAX_ACCOUNT];
    Pagination_t paging;
    node_friend_t *pos;
    int i;
    char choice;
    list_friend_t curos;
    paging.totalRecords=0;
    List_ForEach(head,curos) paging.totalRecords++;
	paging.offset = 0;
	paging.pageSize = FRIEND_PAGE_SIZE;
    //paging.totalRecords = FetchAll_for_Friend_List(head);
    Paging_Locate_FirstPage(head, paging);
    do {
            system("clear");
            //printf("链表长度：%d\n",paging.totalRecords);
            printf(
                    "\n==============================================================\n");
            printf(
                    "********************** Friend  List **********************\n");
            printf("%10s  %20s %15s\n", "account", "Name","status");
            printf(
                    "------------------------------------------------------------------\n");
            Paging_ViewPage_ForEach(head, paging, node_friend_t, pos, i){
                printf("%10s  %20s  %15d     \n",pos->recv_account,pos->nickname,pos->status);
            }

            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[P]revPage | [N]extPage | [Q]uery | [R]eturn");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            fflush(stdin);
            scanf("%c", &choice);
            fflush(stdin);

            switch (choice) {
                case 'q':
                case 'Q':
                    printf("Please enter an account you want to chat with:\n");
                    scanf("%s",account);
                    getchar();
                    Chat(account);
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(head, paging, -1, node_friend_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(head, paging, 1, node_friend_t);
                    }
                    break;
            }
        } while (choice != 'r' && choice != 'R');   
        //链表在客户端退出时进行销毁
}


//注册成功后要加入群列表　
//注册请求　返回一个唯一的群号
//和登录账号用一个　都是唯一的
int register_group_client(int conn_fd)  
{
    int ret;
    recv_t           Package;
    Package.type   = REGISTER_GROUP;
    Package.send_fd= conn_fd; //这个人要被标记为群主
    char nickname[MAX_USERNAME];
    char account[MAX_ACCOUNT];
    system("clear");

    //收集注册信息
    printf("Welcome to register group!\n");
    printf("please enter your group nickname,we will give your a unique account.\n");
    printf("please enter you nickname!\n");
    getchar();
    get_userinfo(nickname,MAX_USERNAME);
    strcpy(Package.message,nickname);
    strcpy(Package.send_Account,gl_account);
    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in register send\n");
        return 0;
    }
/*     if((ret=my_recv(conn_fd,account,MAX_ACCOUNT))<0) //接收账号　在服务器存入数据库
    {
        perror("error in register send\n");
        return 0;
    } */
    usleep(500);//等待下消息到来
    list_messages_t tttt;
    List_ForEach(Message_BOX,tttt)
    {
        if(tttt->type==REGISTER_GROUP)
        {
            strcpy(account,tttt->send_account);//参数为提前定制
            printf("消息盒子中找到　%s %s\n",account,tttt->send_account);
            List_DelNode(tttt); //从消息盒子中获取
            break;
        }
    }
    if(account[0]==ERROR_IN_LOGIN)//这个标记位可以标记所有的错误事件
    {
        perror("error in server data\n");
        return 0;
    }
    printf("This is your Account ,Don't forget it!\n");
    printf("Account:%s\n",account);
    printf("please enter enter key for quit!\n");
    getchar();

    list_group_t tmp=(list_group_t)malloc(sizeof(node_group_t));
    strcpy(tmp->nickname,nickname);
    strcpy(tmp->account,account);
    List_AddTail(group_head,tmp);//加入到群链表

    mp[atoi(account)]=dd++; //分配唯一主键
    List_Init(member[mp[atoi(account)]],node_member_t);//初始化这个群的成员链表
    list_member_t temp=(list_member_t)malloc(sizeof(node_member_t));

    strcpy(temp->account,gl_account);
    strcpy(temp->nickname,fact_name);
    temp->type=OWNER;  //申请者为群主

    List_AddTail(member[mp[atoi(account)]],temp); //加入一个新的成员

    return 1;   //发送正确且收到账号消息返回１
}

int Add_group(int conn_fd)
{
    int              ret=0;
    recv_t           Package;
    Package.type   = ADD_GROUP;
    Package.send_fd= conn_fd;
    strcpy(Package.recv_Acount,gl_account);
    strcpy(Package.message_tmp,fact_name);
    char Account[MAX_ACCOUNT];
    char message[MAX_RECV];   //添加好友时给对方发送的话
    char temp[64];   //就是一个接收消息的缓冲区
    char buf[MAX_USERNAME];
    system("clear");
    getchar();
    printf("Please enter a Group_Account you want to add:");
    get_userinfo(Account,MAX_ACCOUNT);
    strcpy(Package.message,Account); //要加入的账号

    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in add friend send\n");
        return 0;
    }
    else printf("You have joined this group:%s\n",Account);
    getchar();
    
/*     if(recv(conn_fd,buf,sizeof(buf),0)<0) //返回一个名称
    perror("error in recv\n"); */

    list_messages_t tttt;
    List_ForEach(Message_BOX,tttt)
    {
        if(tttt->type==REGISTER_GROUP)
        {
            strcpy(buf,tttt->messages);//参数为提前定制
            List_DelNode(tttt); //从消息盒子中获取
            break;
        }
    }

    list_group_t tmp=(list_group_t)malloc(sizeof(node_group_t));
    strcpy(tmp->account,Account);
    strcpy(tmp->nickname,buf);

    List_AddTail(group_head,tmp);//相当于当先用户加入一个新的群
    //登录阶段加载所有消息记录　以便加入新群后有消息　其实可以加入后再从服务器获取
    //这样效率肯定会更高
    //项目时间快到了只能先赶进度了
    
    if(member[mp_group[atoi(Account)]]==NULL)
    List_Init(member[mp_group[atoi(Account)]],node_member_t);
    list_member_t tmmp=(list_member_t)malloc(sizeof(node_member_t));
    strcpy(tmmp->account,gl_account);
    tmmp->type=COMMON;
    List_AddTail(member[mp_group[atoi(Account)]],tmmp); //成员链表中加入


    if(group_messages[mp_group[atoi(Account)]]==NULL)
    List_Init(group_messages[mp_group[atoi(Account)]],node_group_messages_t);
    list_group_messages_t tmmmp=(list_group_messages_t)malloc(sizeof(node_group_messages_t));
    strcpy(tmmmp->messages,"hello everyone!");
    strcpy(tmmmp->account,gl_account);//好友账号
    tmmmp->type=COMMON;
    List_AddTail(group_messages[mp_group[atoi(Account)]],tmmmp);
    return 1;
}

int Quit_group(int conn_fd)
{
    int              ret=0;
    recv_t           Package;
    Package.type   = QUIT;
    Package.send_fd= conn_fd;
    strcpy(Package.recv_Acount,gl_account);//本身的账号
    char Account[MAX_ACCOUNT];
    char message[MAX_RECV];   //添加好友时给对方发送的话
    char temp[64];   //就是一个接收消息的缓冲区
    char buf[MAX_USERNAME];
    system("clear");
    getchar();
    printf("Please enter a Group_Account you want to quit:\n");
    get_userinfo(Account,MAX_ACCOUNT);
    strcpy(Package.message,Account); //要删除的账号

    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in add friend send\n");
        return 0;
    }
    else printf("You have quited this group:%s\n",Account);
    getchar();
    list_group_t tmp;
    List_ForEach(group_head,tmp)      //从组群中删除这个群
    {
        if(!strcmp(tmp->account,Account))
        {
            List_DelNode(tmp);
            break;
        }
    }
    return 1;
}

int Dissolve(int conn_fd)
{
    char buf[256];
    list_member_t curps;
    list_group_t curpss;
    char account[MAX_ACCOUNT];
    recv_t packet;
    packet.type=DISSOLVE;
    printf("please enter the group you want to dismiss\n");
    scanf("%s",account);
    getchar();
    strcpy(packet.recv_Acount,account);//要解散的群号
        if(send(conn_fd,&packet,sizeof(packet),0)<0)
        perror("error in send dissolve!\n");//服务器应该在两个表中分别删除
                                            //客户端应该在三个链表中分别删除
/*         List_ForEach(member[mp_group[atoi(account)]],curps)
        {
            if(!strcpy(curps->account,gl_account))
            {
                if(curps->type==OWNER)
                {
                    List_DelNode(curps);//在本地删除链表中的数据
                }
            }
        } */
        if(member[mp_group[atoi(account)]]!=NULL)
        List_Destroy(member[mp_group[atoi(account)]],node_member_t);
        if(group_messages[mp_group[atoi(account)]]!=NULL)
        List_Destroy(group_messages[mp_group[atoi(account)]],node_group_messages_t);
        //删除两个链表中的消息
        printf("到达!\n");
        //bzero(curpss,sizeof(node_group_t));
        List_ForEach(group_head,curpss)
        {
            printf("%s %s\n",curpss->account,account);
            if(!strcmp(curpss->account,account))
            {
                printf("找到\n");
                List_DelNode(curpss);  //群成员链表中删除
                break;
            }
        }
    }
    //printf("Lack of permissions\n");


//该函数用在查看群列表以后　可以将某人设为管理员
int Set_Admin(int conn_fd,char * count) //后一个参数为当前群号
{
    list_member_t curps;
    char account[MAX_ACCOUNT];
    recv_t package;
    memset(&package,0,sizeof(package));
    int flag=0;
    List_ForEach(member[mp_group[atoi(count)]],curps){
        if(!strcmp(gl_account,curps->account) && curps->type==OWNER){
            flag=1;
            break;
        }
    }
    if(!flag){
        printf("Lack of permissions!\n");
        return 0;
    }else{
        package.type=SET_ADMIN;
        printf("please enter who you what to talk about as admin:\n");
        scanf("%s",account);
        getchar();
        strcpy(package.message,account);    //成为管理员的账号
        strcpy(package.message_tmp,count);  //群号
        if(send(conn_fd,&package,sizeof(recv_t),0)<0)//在服务器修改这个账号的状态
        perror("error in send!\n");
        List_ForEach(member[mp_group[atoi(count)]],curps){
            if(!strcmp(account,curps->account)){
                curps->type=ADMIN;//在本地设置为管理员
                break;
            }
        }
    }
}

//上下这两个函数是在进入群员列表的时候的选项

int Kicking(int conn_fd,char *count)//第二个参数为群号
{
    char buf;
    list_member_t curps;
    recv_t package;
    package.type=KICKING;
    char account[MAX_ACCOUNT];
    int flag=0;
    printf("please enter a people count you want to kicking\n");
    scanf("%s",account);
    getchar();
/*     List_ForEach(member[mp_group[atoi(count)]],curps){
        if(!strcmp(gl_account,curps->account) && curps->type==OWNER){
            flag=1;
            break;
        }
    }
    if(!flag){    
        printf("You don't have enough permissions!\n");
        getchar();
        return 0;
    } */
    list_member_t cuurps;
    List_ForEach(member[mp_group[atoi(count)]],cuurps)
    {
        if(!strcmp(cuurps->account,account))
        {
            List_DelNode(cuurps); //本地群列表中删除
            break;
        }
    }
    strcpy(package.message,account);//要踢出的人
    strcpy(package.message_tmp,count);//群号
    if(send(conn_fd,&package,sizeof(recv_t),0)<0)
    perror("error in send kicking\n");
    return 0;
}


int recv_file(recv_t *package) //在消息盒子中接收到包　把数据写入文件
{
    int fd=0;                               //这个参数是为了说明新文件的权限
    char buf[MAX_PATH_NAME];
    bzero(buf,sizeof(buf));
    //若果输入路径　简单的前面加前缀就不可以
    strcat(buf,"tmp");
    strcat(buf,package->message_tmp);

    //printf("保存在这里　-> %s\n",buf);
    if((fd=open(buf,O_CREAT|O_RDWR|O_APPEND,S_IRWXU))==-1) //第三个参数的意思就是可读可写可执行
    //把接收到的文件存入当前目录下

    perror("Error opening file!");

    if(write(fd,package->message,strlen(package->message))==-1)
    {
        perror("Error writing file!");
        return -1;
    }
    close(fd);
    return 0;
}


//可以把在登录以后收到的信息包装成一个消息盒子 
//好友请求　好友消息　群聊消息
//在其中处理所有的请求　建立一个消息盒子链表　存储所有数据包　根据其中标记位来辨别请求类型
void *method_client(void *arg)                       
{
    recv_t buf;
    while(1)
    {
        bzero(&buf,sizeof(recv_t));
        if(my_recv_tmp(fact_fd,(char*)&buf,sizeof(recv_t))<0)
        perror("error in recv\n");
        if(buf.type==RECV_FILE)
        {
            if(recv_file(&buf)<0)
            {
                printf("error in recv file\n");
            }
            continue;
        }
        printf("消息盒子收到消息　%d %s\n",buf.type,buf.send_Account);
        list_messages_t temp = (list_messages_t)malloc(sizeof(node_messages_t));
        temp->type=buf.type;//标记位　
        strcpy(temp->send_account,buf.send_Account);//发送者
        strcpy(temp->messages,buf.message);//消息
        strcpy(temp->nickname,buf.message_tmp);//昵称
        strcpy(temp->recv_account,buf.recv_Acount);//就是本人账号
        List_AddTail(Message_BOX,temp);
    }
}


int show_group_member(char *account)
{
    //char account[MAX_ACCOUNT];
    Pagination_t paging;
    node_member_t *pos;
    int i;
    char choice;
    list_member_t curos;
    //printf("成员 %s map : %d\n",account,mp_group[atoi(account)]);
    paging.totalRecords=0;
    printf("遍历链表开始\n");
    if(member[mp_group[atoi(account)]]==NULL)
    List_Init(member[mp_group[atoi(account)]],node_member_t);
    List_ForEach(member[mp_group[atoi(account)]],curos) paging.totalRecords++;
    printf("遍历链表结束\n");
	paging.offset = 0;
	paging.pageSize = FRIEND_PAGE_SIZE;
    //group_head  在登录时已经初始化
    //paging.totalRecords = FetchAll_for_Friend_List(head);
    do {
            Paging_Locate_FirstPage(member[mp_group[atoi(account)]], paging);
            system("clear");
            //printf("链表长度：%d\n",paging.totalRecords);
            printf("\001\033[1m\002");
            printf("\033[34m");
            printf(
                    "\n==============================================================\n");
            printf(
                    "********************** Group Member List **********************\n");
            printf("%10s \n", "Group_account");
            printf(
                    "------------------------------------------------------------------\n");
            Paging_ViewPage_ForEach(member[mp_group[atoi(account)]], paging, node_member_t, pos, i){
                printf("%10s\n",pos->account);
            }

            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[C]hat | [K]ick |[S]et_admin\n");
            printf(
                    "[P]revPage | [N]extPage | [R]eturn");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            printf("\001\033[0m\002");
            fflush(stdin);
            scanf("%c", &choice);
            getchar();

            switch (choice) {
                case 'c':
                case 'C':
                    printf("这个功能不一定能用\n");
                    printf("Please enter an account you want to chat with:\n");
                    scanf("%s",account);
                    getchar();
                    Chat(account); //通过群进行聊天
                    break;
                case 'k':
                case 'K':
                    paging.totalRecords--;
                    Kicking(fact_fd,account); //踢人
                    break;
                case 's':
                case 'S':
                    printf("please enter who you what to set him to admin\n");
                    scanf("%s",account);
                    getchar();
                    Set_Admin(fact_fd,account); //把某人设置为管理员
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(member[mp_group[atoi(account)]], paging, -1, node_member_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(member[mp_group[atoi(account)]], paging, 1, node_member_t);
                    }
                    break;
            }
        } while (choice != 'r' && choice != 'R');  
}


int send_group_messages(char *account,char *Message)//第一个参数为群号　第二个参数为消息
{
    recv_t packge;
    bzero(&packge,sizeof(packge));
    packge.type=SEND_GROUP_MESSAGES;
    strcpy(packge.message,Message);//消息
    strcpy(packge.recv_Acount,account);//群号
    strcpy(packge.send_Account,gl_account);//发送者
    strcpy(packge.message_tmp,fact_name);//昵称
    if(send(fact_fd,&packge,sizeof(recv_t),0)<0)
    perror("error in seng group messages\n");
    
    //接下来把消息存入本地邻接表
    //服务器把消息存入数据库
/*     list_group_messages_t tmp=(list_group_messages_t)malloc(sizeof(node_group_messages_t));
    strcpy(tmp->messages,Message);
    strcpy(tmp->nickname,fact_name);
    List_AddTail(group_messages[mp_group[atoi(account)]],tmp); */
    return 0;
}


//这个函数的作用是进行群聊天
int Group_Chat(char *account)//参数为想参与群聊的群号
{
    char Message[MAX_RECV];
    Pagination_t paging;
    node_group_messages_t *pos;
    char acc_tmp[MAX_ACCOUNT];
    strcpy(acc_tmp,account);
    int i;
    char choice;
    int flag=0;
    list_messages_t curos; //用于遍历消息盒子
    list_group_messages_t cur; //用于遍历本群消息总数
    paging.totalRecords=0;
    if(group_messages[mp_group[atoi(account)]]==NULL) //防止一个新注册的群　消息链表还未初始化
    {
        printf("初始化\n");
        mp_group[atoi(account)]=++dd;
        List_Init(group_messages[mp_group[atoi(account)]],node_group_messages_t);
    }
    List_ForEach(group_messages[mp_group[atoi(account)]],cur) paging.totalRecords++;
    //遍历消息链表
	paging.offset = paging.totalRecords;
	paging.pageSize = MESSADES_PAGE_SIZE;
    Paging_Locate_FirstPage(group_messages[mp_group[atoi(account)]], paging);
            while(1){
            flag=0;//退出此函数标记
            system("clear");
            //printf("链表长度：%d\n",paging.totalRecords);

            //在消息盒子中查找是否有正在发消息的好友发送来的消息
            List_ForEach(Message_BOX,curos) 
            {
                //消息肯定是发送者是正在聊天的好友的账号               //这个参数为群号
                if(curos->type==SEND_GROUP_MESSAGES && !strcmp(curos->send_account,account))
                {
                    list_group_messages_t temp = (list_group_messages_t)malloc(sizeof(node_group_messages_t));
                    strcpy(temp->messages,curos->messages);
                    strcpy(temp->nickname,curos->nickname);
                    List_AddTail(group_messages[mp_group[atoi(account)]],temp);
                    paging.totalRecords+=1;//更新消息链表
                    List_FreeNode(curos); //这个消息已经载入消息链表　可以删除了
                }
            }
/*             List_ForEach(group_messages[mp_group[atoi(account)]],cur)
            {
                cout << cur->messages << endl;
            } */

            printf("\001\033[1m\002");
            printf("\033[34m");
            printf(
                    "\n==================================================================\n");
            printf(
                    "**************************** %s ****************************\n",account);//有消息可以用这个　Messages[mp[account]]->nicknam
            printf(
                    "------------------------------------------------------------------\n");
            printf("\001\033[0m\002");
                    //printf("((((%d,%s,%d\n",mp[atoi(acc_tmp)],acc_tmp,strlen(acc_tmp));
            Paging_ViewPage_ForEach(group_messages[mp_group[atoi(account)]], paging, node_group_messages_t, pos, i){
                //链表中名称必为好友昵称
                //printf("%s :%s :\n",pos->nickname,fact_name);
                if(strcmp(pos->nickname,fact_name))//怎么比都可以
                {
                    printf("\033[32m %-65s \033[0m \n",pos->nickname);
                    printf("\033[32m %-65s \033[0m \n",pos->messages);
                }else{
                    printf("\033[35m %65s \033[0m \n",fact_name);
                    printf("\033[35m %65s \033[0m \n",pos->messages);
                }
                putchar('\n');
            }

            printf("\001\033[1m\002");
            printf("\033[34m");
            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[P]revPage | [N]extPage | [I]uput | [R]eturn");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            fflush(stdin);
            printf("\001\033[0m\002");
            scanf("%c", &choice);
            getchar();
            fflush(stdin);

            switch (choice) {
                case 'I':
                case 'i':
                {//这里需要修改　
                    printf("please enter:\n");
                    cin.getline(Message,sizeof(Message)); 
                    list_group_messages_t temp=(list_group_messages_t)malloc(sizeof(node_group_messages_t));
                    strcpy(temp->messages,Message);
                    List_AddTail(group_messages[mp_group[atoi(account)]],temp);//加入到消息链表
                    paging.totalRecords++;
                    send_group_messages(account,Message);
                }
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(group_messages[mp_group[atoi(account)]], paging, -1, node_group_messages_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(group_messages[mp_group[atoi(account)]], paging, 1, node_group_messages_t);
                    }
                    break;
                case 'r':
                case 'R':
                    flag=1;
                    break;
            }
            if(flag) break;
        } 
}

int show_group_list()
{
    char account[MAX_ACCOUNT];
    Pagination_t paging;
    node_group_t *pos;
    int i;
    char choice;
    list_group_t curos;
    paging.totalRecords=0;
    List_ForEach(group_head,curos) paging.totalRecords++;
	paging.offset = 0;
	paging.pageSize = FRIEND_PAGE_SIZE;
    //group_head  在登录时已经初始化
    //paging.totalRecords = FetchAll_for_Friend_List(head);
    Paging_Locate_FirstPage(group_head, paging);
    do {
            //paging.totalRecords=0;
            //List_ForEach(group_head,curos) paging.totalRecords++;
            system("clear");
            //printf("链表长度：%d\n",paging.totalRecords);
            printf(
                    "\n==============================================================\n");
            printf(
                    "********************** Group  List **********************\n");
            printf("%15s  %20s\n", "account", "Group_Name");
            printf(
                    "------------------------------------------------------------------\n");
            Paging_ViewPage_ForEach(group_head, paging, node_group_t, pos, i){
                printf("%10s      %20s\n",pos->account,pos->nickname);
            }

            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[Q]uery | [C]hat | [D]issolve \n");    
            printf(
                    "[E]xit_from_group\n");           
            printf(
                    "[P]revPage | [N]extPage | [R]eturn\n");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            fflush(stdin);
            scanf("%c", &choice);
            //getchar();
            switch (choice) {
                case 'c':
                case 'C':
                    printf("Please enter an account you want to chat with:\n");
                    scanf("%s",account);
                    Group_Chat(account);
                    break;
                case 'Q':
                case 'q':
                    //查看群成员列表
                    printf("Please enter an account you want to query with:\n");
                    scanf("%s",account);
                    show_group_member(account);
                    break;
                case 'd':
                case 'D':
                    Dissolve(fact_fd); //解散群
                    break;
                case 'e':
                case 'E':
                    paging.totalRecords--;
                    Quit_group(fact_fd);//退出群
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(group_head, paging, -1, node_group_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(group_head, paging, 1, node_group_t);
                    }
                    break;
            }
        } while (choice != 'r' && choice != 'R');   
}



void *send_file(void *arg) //发送文件时重新开一个线程 
{
    int fd=0;              //文件描述符
    struct stat buffer;
    recv_t package;
    recv_file_t *recv_file=(recv_file_t*)arg;
    bzero(&buffer,sizeof(struct stat));


    if(lstat(recv_file->path,&buffer)<0){ //判断文件是否存在
        printf("File does not exxist!\n");
        printf("please enter key to quit!\n");
        getchar();
        return 0;
    }
    printf("%s\n",recv_file->path);
    if((fd=open(recv_file->path,O_RDONLY))==-1){//确定文件已经存在
        perror("open file error！\n");
        printf("please enter key to quit!\n");
        getchar();
        return 0;
    }

    printf("给%s发送消息\n",recv_file->count);

    while(read(fd,package.message,MAX_RECV-1)>0) //循环开始发送文件
    {
        strcpy(package.message_tmp,recv_file->path);
        strcpy(package.recv_Acount,recv_file->count);
        package.type=SEND_FILE;
        //printf("%s\n",package.message);
        strcat(package.message,"\0");
        if(send(fact_fd,&package,sizeof(recv_t),0)<0)
        perror("error in send file\n");
        bzero(&package.message,MAX_RECV);//清空缓冲区
    }

    bzero(&package,sizeof(recv_t));
    strcpy(package.recv_Acount,recv_file->count); 
    package.type=EOF_OF_BOX;  //发包结束符
    if(send(fact_fd,&package,sizeof(recv_t),0)<0)
    perror("error in send file\n");

    printf("Successfully sent!\n");
    printf("please enter key to quit!\n");
    close(fd);
    return 0;
}