#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>
#include<pthread.h>
#include<stdio.h>
#include<mysql/mysql.h>
#include"Data.h"
list_status_t status_per;

void send_data(int conn_fd,const char *string,int len) //传入一个连接套接字和字符串数据   
{
    int ree=0;
    printf("%s\n",string);
    //if((ree=send(conn_fd,string,strlen(string),0))<0)
    
    //这里的错误很隐晦 strlen会导致传的包的大小出现问题
    if((ree=send(conn_fd,string,len,0))<0)
    {
        perror("send");
        exit(1);
    }
    printf("返回值%d\n",ree);
}

void Delete_for_friend_third(char *a,char *b,char *c) //为了和并出一个唯一的字符串删除好友关系
{
    
    if(strcmp(a,b)<=0)
    {
        strcat(c,a);
        strcat(c,b);
    }else
    {
        strcat(c,b);
        strcat(c,a);
    }  
}

int login(recv_t *sock,MYSQL *mysql)  //sock_fd是要被发送数据的套接字
{
    int ret;
    char recv_buf[MAX_USERNAME];//登录时默认使用字符串
    int flag_recv=USERNAME;
    char buf[256];
    sprintf(buf,"select *from Data where Account = %s",sock->send_Account);
    mysql_query(mysql,buf);
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);
    printf("%s || %s\n",sock->message,row[1]);
    mysql_free_result(result);
    MYSQL_RES *res=NULL;
    int row_in_messages_box=0;
    if(!strcmp(sock->message,row[1]))//在数据库中检测账号密码是否匹配 返回名称　密码在message中
    {
        send_data(sock->send_fd,(char*)row[3],MAX_USERNAME);//发送名称
        sprintf(buf,"update Data set status = \"1\" where Account = \"%s\"",sock->send_Account);
        mysql_query(mysql,buf); //改变登录状态
        //查询消息盒子 把离线期间发送给send_account的消息提取并发送
        sprintf(buf,"select *from messages_box where recv_account = '%s'",sock->send_Account);
        mysql_query(mysql,buf);
        res=mysql_store_result(mysql);
        if(res==NULL)  //等于空就是出现错误　成功不会为NULL 查询的行为０也不会为NULL
        {
            perror("error in mysql_store_result\n");
            return 0;
        }
        //先发送一个代表消息盒子是否有信息的包　客户端做出接收　
        //两种情况分情况编写代码 因为发信息不知道什么时候结束　只能在结束时发送一个代表消息结束的包
        if((row_in_messages_box=mysql_num_rows(res))==0)
        {
            printf("已发送1！\n");
            send_data(sock->send_fd,BOX_NO_MESSAGES,MAX_RECV);
        }else
        {
            printf("已发送2！\n");
            send_data(sock->send_fd,BOX_HAVE_MESSAGS,MAX_RECV);
        }
        //printf("标志消息盒子　是否有数据的包发送成功  %d\n",row_in_messages_box);
        //开始发送消息
        Box_t box;
        memset(&box,0,sizeof(Box_t));
        int flag=0;
        if(row_in_messages_box==0) flag=1;
        printf("row_in_messasd %d\n",row_in_messages_box);
        while(row_in_messages_box--)
        {

            row=mysql_fetch_row(res);
            box.type=ADD_FRIENDS;      //时间类型　离线消息不止添加好友
            strcpy(box.message,row[3]);//消息
            strcpy(box.account,row[1]);//发送者
            if(send(sock->send_fd,&box,sizeof(Box_t),0)<0)
            perror("error in send\n");
            sprintf(buf,"delete from messages_box where recv_account = '%s' and send_acount = '%s' and message = '%s'",
            sock->send_Account,box.account,box.message);
            //printf("%s\n",buf);
            mysql_query(mysql,buf);
        }
        if(flag!=1)
        {
            printf("运行到这\n");
            box.type=EOF_OF_BOX;
            strcpy(box.message,row[3]);
            send(sock->send_fd,&box,sizeof(Box_t),0);
        }
        //printf("全部信息发送完成\n");
    }
    else 
    ;//send_data(sock->send_fd,"@@@",MAX_USERNAME);//密码账号不匹配　返回错误
    mysql_free_result(res);

    //发送好友列表的函数所需要的值登录函数中已设置　所以这个数据包可直接使用　
    //有效位为其中的　send_Account 与 send_fd 
    //谁发的　以及　套接字是多少
    //printf("函数进行到这里数据库查找数据\n");
    printf("开始加载\n");
    List_friends_server(sock,mysql);
    printf("好友列表加载完成\n");
    //从消息记录中查找账号所对应的消息记录　标记位为ower_account


    //根据登陆者的账号在数据库中进行匹配
    sprintf(buf,"select *from messages_record where ower_account = '%s'",sock->send_Account);
    mysql_query(mysql,buf);
    MYSQL_RES *resu=mysql_store_result(mysql);
    MYSQL_ROW rowwor;
    //rowwor=mysql_fetch_row(resu);
    int row_in_messages_record;
    Box_t box;
    //根据数据库中有无登录者消息记录　先发送一个起始包　
    printf("运行到发送消息标记包\n");
    if((row_in_messages_record=mysql_num_rows(resu))==0)
    {
        send_data(sock->send_fd,BOX_NO_MESSAGES,MAX_USERNAME);
        return 0; //不退出会在后面多发一个包
    }else
    {
        send_data(sock->send_fd,BOX_HAVE_MESSAGS,MAX_USERNAME);
    }
    printf("row_in %d\n",row_in_messages_record);
    while(row_in_messages_record--)
    {
        bzero(&box,sizeof(Box_t));
        rowwor=mysql_fetch_row(resu); //获取消息信息
        //printf("%s %s\n",sock->send_Account,rowwor[1]);
/*         if(!strcmp(sock->send_Account,rowwor[1])) //查找到与之聊天的好友的正确账号　//为查询昵称
        sprintf(buf,"select *from Data where Account = '%s'",rowwor[2]);
        else  */ 
        sprintf(buf,"select *from Data where Account = '%s'",rowwor[1]);
        mysql_query(mysql,buf);
        MYSQL_RES *resu_tmp = mysql_store_result(mysql);
        MYSQL_ROW rowwor_tmp=mysql_fetch_row(resu_tmp);
        strcpy(box.message,rowwor[3]); //消息
        if(!strcmp(rowwor[1],sock->send_Account))
        strcpy(box.account,rowwor[2]); //必须保证是好友账号
        else 
        strcpy(box.account,rowwor[1]); 
        //printf("%s\n",box.account);
        strcpy(box.usename,rowwor_tmp[3]);//发送者昵称
        //printf("%s\n",rowwor_tmp[3]);
        box.type=SEND_MESSAGES; //消息类型
        mysql_free_result(resu_tmp);
        if(send(sock->send_fd,&box,sizeof(Box_t),0)<0)
        perror("error in send a message when logging in\n");

    }
    box.type=EOF_OF_BOX;
    if(send(sock->send_fd,&box,sizeof(Box_t),0)<0) //发送一个结束包
    perror("error in send a message when logging in\n");




    //从这里开始发送群记录和群消息
    int ddd=0;
    recv_t tmp_tmp;     //客户端用一个while接收
    bzero(&tmp_tmp,sizeof(recv_t));
    bzero(buf,sizeof(buf));
    sprintf(buf,"select *from group_list where member_account = '%s'",sock->send_Account);
    mysql_query(mysql,buf);
    MYSQL_RES *res_tmp=mysql_store_result(mysql);
    MYSQL_ROW rowwo;
    if((ddd=mysql_num_rows(res_tmp))==0) //群为零　也就不用发送消息记录了
    {
        tmp_tmp.type=NULL_OF_GROUP;//直接发送标记符
        send(sock->send_fd,&tmp_tmp,sizeof(recv_t),0); //客户端是一个while循环接收,所以一个足够
        return 0;
    }else{
        while(ddd--) //ddd为所有的群的总数
        {
            int ret=0;
            rowwo=mysql_fetch_row(res_tmp);
            bzero(&tmp_tmp,sizeof(recv_t));
            strcpy(tmp_tmp.message,rowwo[0]);//所有包含本用户的群
            strcpy(tmp_tmp.message_tmp,rowwo[2]);//昵称
            //tmp_tmp.type=atoi(rowwo[3]);//在群中的职位
            strcpy(tmp_tmp.type,rowwo[3]);
            if((ret=send(sock->send_fd,&tmp_tmp,sizeof(recv_t),0)<0))
            {
                perror("error in send group\n");
            }
        }
        bzero(&tmp_tmp,sizeof(recv_t)); //发送一个结束包
        tmp_tmp.type=EOF_OF_BOX;
        printf("结束包已发送\n");
        if((ret=send(sock->send_fd,&tmp_tmp,sizeof(recv_t),0)<0))
        {
            perror("error in send group\n");
        }
    }
    mysql_free_result(res_tmp);

    //开始发送消息
    sprintf(buf,"select *from group_messsges_list",sock->send_Account);
    mysql_query(mysql,buf);
    res_tmp=mysql_store_result(mysql);
    int fff=mysql_num_rows(res_tmp); //获取所有消息　发送给客户端
    printf("消息记录共有：　%d\n",fff);
    while(fff--)
    {
        //在客户端在接收的时候根据
        bzero(&tmp_tmp,sizeof(recv_t));
        rowwo=mysql_fetch_row(res_tmp);
        strcpy(tmp_tmp.message,row[2]);//群名
        strcpy(tmp_tmp.message_tmp,row[3]);//消息
        strcpy(tmp_tmp.recv_Acount,row[0]);//群账号
        strcpy(tmp_tmp.send_Account,row[1]);//好友账号
        //tmp_tmp.type=atoi(row[4]);//好友的等级　群主　管理员
        strcpy(tmp_tmp.type,row[4]);
        if(send(sock->send_fd,&tmp_tmp,sizeof(recv_t),0)<0)
        {
            perror("error in send\n");
        }
    }
    bzero(&tmp_tmp,sizeof(recv_t)); //发一个结束包
    tmp_tmp.type=EOF_OF_BOX;
    printf("消息结束包已发送\n");
    if(send(sock->send_fd,&tmp_tmp,sizeof(recv_t),0)<0)
    {
        perror("error in send\n");
    }
    return 0;
}

int register_server(recv_t * sock,MYSQL *mysql)
{
    char account[MAX_ACCOUNT];
    char buf[256];
    memset(account,0,sizeof(account));
    mysql_query(mysql,"select *from Account");
    //perror("error in mysql_query\n");
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);
    //itoa(row[0]+1,account,10);    //atoi字符串转数字
    //数字转化为字符串必须用sprintf itoa不标准
    sprintf(account,"%d",atoi(row[0])+1);
    sprintf(buf,"update Account set Account = \"%s\" where Account = \"%s\"",account,row[0]);
    mysql_query(mysql,buf);
    send_data(sock->send_fd,account,MAX_ACCOUNT);//注册时返回一个账号                                       //存一次昵称
    sprintf(buf,"insert into Data values('%s','%s','%s','%s',0,%d)",
    account,sock->message,sock->message_tmp,sock->recv_Acount,sock->send_fd);
    printf("%s\n",buf);
    mysql_query(mysql,buf);
    mysql_free_result(result);
}

int Retrieve_server(recv_t *sock,MYSQL *mysql)
{
    int ret;
    char recv_buf[MAX_USERNAME];
    char buf[256];
    sprintf(buf,"select *from Data where Account = %s",sock->send_Account);
    mysql_query(mysql,buf);
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row;
    row=mysql_fetch_row(result);
    if(!strcmp(sock->message_tmp,row[2]))
    {
        sprintf(buf,"update Data set password = \"%s\" where Account = \"%s\"",sock->message,sock->send_Account);
        mysql_query(mysql,buf);
        send_data(sock->send_fd,"y",32);
    }
    else 
    send_data(sock->send_fd,"@@@",32);
}

int add_friend_server(recv_t *sock,MYSQL *mysql)
{
    int ret;
    char recv_buf[MAX_USERNAME];
    char buf[256];
    sprintf(buf,"select *from Data where Account = %s",sock->recv_Acount);
    mysql_query(mysql,buf);
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);
    int tmp=atoi(row[5]);
    if(atoi(row[4])==1)  //在线
    {
        //printf("11\n");
        if(send(tmp,sock,sizeof(recv_t),0)<0)  //根据账号查找到接收者的套接字
        perror("error in send\n");//需要在线消息盒子　否则无法实现
    }else  //不在线把数据放到消息盒子
    {
        //printf("212\n");
        sprintf(buf,"insert into messages_box values('%d','%s','%s','%s')",tmp,sock->send_Account,sock->recv_Acount,sock->message);
        //printf("%s\n",buf);
        mysql_query(mysql,buf);
    }
    //成功后不发送消息
}

int add_friend_server_already_agree(recv_t *sock,MYSQL *mysql)//向朋友数据库加入消息
{
    //friend数据表中第三项　是为了在删除时仅删除一项就把一对好友关系进行删除　
    //这个函数只需要操作下数据库就好
    char buf[512];
    char unique_for_del[64];
    Delete_for_friend_third(sock->recv_Acount,sock->send_Account,unique_for_del);
    unique_for_del[strlen(sock->recv_Acount)+strlen(sock->send_Account)+1]='\0';
    //printf("%s\n",unique_for_del);
    sprintf(buf,"insert into friend values('%s','%s','%s')",sock->recv_Acount,sock->send_Account,unique_for_del);
    //printf("加入数据库:%s\n",buf);
    mysql_query(mysql,buf);
    return 1;
}

int del_friend_server(recv_t *sock,MYSQL *mysql)
{
    char buf[256];
    char unique_for_del[64];
    memset(unique_for_del,0,sizeof(unique_for_del)); 
    //再说一遍　初始化及其重要　其中很可能有一些废数据
    Delete_for_friend_third(sock->recv_Acount,sock->send_Account,unique_for_del);
    unique_for_del[strlen(sock->recv_Acount)+strlen(sock->send_Account)+1]='\0';
    //printf("%s %s %s\n",sock->recv_Acount,sock->send_Account,unique_for_del);
    sprintf(buf,"delete from friend where del = '%s'",unique_for_del);
    printf("%s\n",buf);
    mysql_query(mysql,buf);
    return 1;
}

int List_friends_server(recv_t *sock,MYSQL *mysql) //因为数据库表建的不好　导致查找效率较低
{
    printf("进入到加载好友列表\n");
    recv_t packet;
    packet.type=LIST_FRIENDS;    //区别与EOF包的差别
    char send_account[MAX_ACCOUNT];  //请求好友列表者　
    strcpy(send_account,sock->send_Account);
    char buf[256];
    sprintf(buf,"select *from friend where account1 = '%s'",sock->send_Account);
    mysql_query(mysql,buf);
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_RES *res=NULL;
    int number=mysql_num_rows(result);
    MYSQL_ROW row,wor;
    printf("第一遍搜索：%d:\n",number);
    while(number--)//第一遍搜索的好友总数
    {
        row=mysql_fetch_row(result);
        //printf("开始搜索好友！\n");
        sprintf(buf,"select *from Data where Account = '%s'",row[1]);//每一个好友的信息
        //printf("%s\n",buf);
        mysql_query(mysql,buf);
        res=mysql_store_result(mysql);
        wor=mysql_fetch_row(res);
        strcpy(packet.message,wor[3]);//昵称
        strcpy(packet.message_tmp,row[1]);//好友账号
        //printf("%s\n",packet.message); //测试用
        strcpy(packet.conn_fd,wor[4]);//是否在线
        strcpy(packet.send_fd,wor[5]);//好友套接字
/*         packet.conn_fd=atoi(wor[4]);
        packet.send_fd=atoi(wor[5]); */
        printf("好友信息发送 昵称 %s\n",packet.message);
        if((send(sock->send_fd,(char*)&packet,sizeof(recv_t),0))<0)
        perror("error in list_friend send\n");
        //printf("hello!\n");
    }
    mysql_free_result(result);
    mysql_free_result(res);  //释放一遍空间
    //开始第二遍搜索　数据库表建的不好　不然可以一遍ok的


    sprintf(buf,"select *from friend where account2 = '%s'",sock->send_Account);
    mysql_query(mysql,buf);
    result = mysql_store_result(mysql);
    res=NULL;
    number=mysql_num_rows(result);   //获取好友
    printf("第二遍搜索：%d:\n",number);
    while(number--)//第二遍搜索的好友总数
    {
        row=mysql_fetch_row(result);
        bzero(&packet,sizeof(recv_t));
        sprintf(buf,"select *from Data where Account = '%s'",row[0]);//每一个好友的信息
        mysql_query(mysql,buf);
        res=mysql_store_result(mysql);
        wor=mysql_fetch_row(res);
        strcpy(packet.message,wor[3]);//昵称
        strcpy(packet.message_tmp,row[1]);//好友账号
        strcpy(packet.conn_fd,wor[4]);//是否在线
        strcpy(packet.send_fd,wor[5]);//好友套接字
        printf("好友信息发送 昵称 %s\n",packet.message);
        if((send(sock->send_fd,(char *)&packet,sizeof(recv_t),0))<0)
        perror("error in list_friend send\n");
        //printf("hello!\n");
    }
    
    bzero(&packet,sizeof(recv_t));
    packet.type=EOF_OF_BOX;//好友消息的结束包
    printf("已发送 好友结束包\n");
    if((send(sock->send_fd,(char*)&packet,sizeof(recv_t),0))<0)
    perror("error in EOF list_friend\n");
    mysql_free_result(result);
    mysql_free_result(res);
    return 1;
}

//接收到客户端的发送的消息　在数据库中判断接收者是否在线　在线直接发送　不然不用管了　
//等对方上线时消息自动加载　写完后记得修改状态的问题　最好正常结束发一个结束包　修改请求
int send_messages_server(recv_t *sock,MYSQL *mysql)
{
    //判断是否在线　在线发送　然后把消息加入离线消息盒子(在线与不在线都要加入离线消息盒子　保存聊天信息)
    int ret;
    char recv_buf[MAX_USERNAME];
    char buf[256];
    sprintf(buf,"select *from Data where Account = '%s'",sock->recv_Acount);
    mysql_query(mysql,buf);
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);
    if(atoi(row[4])==1)//在线　直接发送　消息盒子接收 //其中消息进入后直接载入　不区分已读未读
    {
        recv_t package;
        strcpy(package.send_Account,sock->send_Account);
        strcpy(package.recv_Acount,sock->recv_Acount);
        strcpy(package.message_tmp,row[3]);
        strcpy(package.message,sock->message);
        package.type=SEND_MESSAGES;

        int flag=0;
        list_status_t curps;
        List_ForEach(status_per,curps)
        {
            printf("%s %s\n",sock->recv_Acount,curps->account);
            if(!strcmp(sock->recv_Acount,curps->account))
            {
                printf("daodai!\n");
                flag=curps->fdd;//在在线链表中寻找套接字
                break;
            }
        }

        if(send(flag,&package,sizeof(recv_t),0)<0) 
        {
            perror("error in server send friend message\n");
        }
    }
    //开始把消息存入数据库　做标记　为好友信息
    mysql_free_result(result);
    
    sprintf(buf,"insert into messages_record values('%s','%s','%s','%s')",
    sock->send_Account,sock->send_Account,sock->recv_Acount,sock->message);
    printf("%s\n",buf);
    mysql_query(mysql,buf);

    sprintf(buf,"insert into messages_record values('%s','%s','%s','%s')",
    sock->recv_Acount,sock->send_Account,sock->recv_Acount,sock->message);
    printf("%s\n",buf);
    mysql_query(mysql,buf);//向消息记录数据库中加入消息　消息有两份
    //根据　ower_account 位来标记消息的所属者是谁　从而在登录时进行加载
}


int register_group_server(recv_t *sock,MYSQL *mysql)
{
    char account[MAX_ACCOUNT];
    char buf[256];
    recv_t packet;
    memset(account,0,sizeof(account));
    mysql_query(mysql,"select *from Account");
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);

    sprintf(account,"%d",atoi(row[0])+1);
    sprintf(buf,"update Account set Account = \"%s\" where Account = \"%s\"",account,row[0]);
    mysql_query(mysql,buf);//更新数据　保证不重复
    mysql_free_result(result);
    sprintf(buf,"insert into group_list values('%s','%s','%s','%d')",
    account,sock->send_Account,sock->message,OWNER);//把这个群存入数据库
    mysql_query(mysql,buf);

    printf("注册得到的群号: %s\n",account);
    strcpy(packet.send_Account,account);
    packet.type=REGISTER_GROUP;
    if(send(sock->send_fd,&packet,sizeof(recv_t),0)<0)
    perror("error in register group !\n");
}

int Add_group_server(recv_t *sock,MYSQL *mysql)
{
    char account[MAX_ACCOUNT];
    char buf[256];
    recv_t packet;
    memset(account,0,sizeof(account));
    sprintf(buf,"insert into group_list values('%s','%s','%s','%d')",
    sock->message,sock->recv_Acount,sock->message_tmp,COMMON);//成员姓名
    
    printf("%s\n",buf);
    mysql_query(mysql,buf);
    mysql_query(mysql,"select *from group_list");
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);
    strcpy(packet.message,row[2]);
    packet.type=ADD_GROUP;
    if(send(sock->send_fd,&packet,sizeof(recv_t),0)<0) //发送群名称
    perror("error in send!(add group server)\n");

    return 1;
}

int Quit_group_server(recv_t *sock,MYSQL *mysql)
{
    char buf[256];
    sprintf(buf,"delete from group_list where member_account = '%s' and group_account = '%s'",
    sock->recv_Acount,sock->message);
    mysql_query(mysql,buf);
}

int Dissolve_server(recv_t *sock,MYSQL *mysql)//数据库中删除相关数据　×两张表的数据
{
    int ret;
    char recv_buf[MAX_USERNAME];//登录时默认使用字符串
    int flag_recv=USERNAME;
    char buf[256];
    sprintf(buf,"delete from group_list where group_account = '%s'",sock->recv_Acount);
    printf("%s\n",buf);
    mysql_query(mysql,buf);
    //直接删除即可　已经在客户端检测过有权限　
    sprintf(buf,"delete from group_messsges_list where group_account = '%s'",sock->recv_Acount);
    printf("%s\n",buf);
    mysql_query(mysql,buf);
    //两张表中的数据都要删除
    //在这里其实可以改良　就是删除的时候给每一个成员发送一个包　消息盒子接收　随即删除
    //但是没时间了
    return 0;
}

int Set_Admin_server(recv_t *sock,MYSQL *mysql)
{
    char buf[256];
    memset(buf,0,sizeof(buf));
    sprintf(buf,"update group_list set type = '%d' where group_account = '%s' and member_account'%s'",
    ADMIN,sock->message_tmp,sock->message);
    mysql_query(mysql,buf);
    //其实应该给每一个群员发送一个更新消息
    return 0;
}

int Kicking_server(recv_t *sock,MYSQL *mysql)
{
    char buf[256];
    sprintf(buf,"delete group_list where group_account = '%s' and member_account = '%s'",
    sock->message_tmp,sock->message);//删除掉此人
    mysql_query(mysql,buf);
}

int *solve(void *arg)
{
    MYSQL mysql;
    mysql_init(&mysql);  //初始化一个句柄
    mysql_library_init(0,NULL,NULL);//初始化数据库
    mysql_real_connect(&mysql,"127.0.0.1","root","lzl213260C","Login_Data",0,NULL,0);//连接数据库
    mysql_set_character_set(&mysql,"utf8");//调整为中文字符
    recv_t *recv_buf=(recv_t *)arg;
    int recv_flag=recv_buf->type;
    printf("消息号码　: %d\n",recv_flag);
    switch (recv_flag)
    {
        case LOGIN :
            login(recv_buf,&mysql);
            break;
        case REGISTER :
            register_server(recv_buf,&mysql);
            break;
        case RETRIEVE:
            Retrieve_server(recv_buf,&mysql);
            break;
        case ADD_FRIENDS:
            add_friend_server(recv_buf,&mysql);
            break;
        case ADD_FRIENDS_QUERY:
            add_friend_server_already_agree(recv_buf,&mysql);
            break;
        case DEL_FRIENDS:
            del_friend_server(recv_buf,&mysql);
            break;
        case LIST_FRIENDS:
            List_friends_server(recv_buf,&mysql);
            break;
        case SEND_MESSAGES:
            send_messages_server(recv_buf,&mysql);
            break;
        case REGISTER_GROUP:
            register_group_server(recv_buf,&mysql);
            break;
        case ADD_GROUP:
            Add_group_server(recv_buf,&mysql);
            break;
        case QUIT:
            Quit_group_server(recv_buf,&mysql);
            break;
        case DISSOLVE:
            Dissolve_server(recv_buf,&mysql);
            break;
        case SET_ADMIN:
            Set_Admin_server(recv_buf,&mysql);
            break;
        case KICKING:
            Kicking_server(recv_buf,&mysql);
            break;
        default:
            printf("error\n");
            break;
    }
    printf("end of pthread!\n");
    struct epoll_event ev;
    ev.data.fd = recv_buf->conn_fd;
    ev.events = EPOLLIN | EPOLLONESHOT;
    //设置这个的目的是客户端在挂掉以后会发送一个信息　LT模式下没有接到包会不停的发　就会导致服务器epoll收到很多消息
    //解决方案是开始时事件类型改为那三个　然后设置EPOLLONESHOT　一个套接字只接受一次信息　在线程中在加上即可
    epoll_ctl(recv_buf->epfd, EPOLL_CTL_MOD,recv_buf->conn_fd, &ev);
    mysql_close(&mysql);
    free(recv_buf);
}