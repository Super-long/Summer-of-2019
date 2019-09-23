#include"incident.h"

int main(int argc,char **argv)  //暂时无全局变量
{
    setbuf(stdin,NULL);
    pthread_t pp_tmp;//发送文件要重新开一个线程　防止阻塞本账号与其他人的聊天
    int i;
    int ret;
    int conn_fd;
    int serv_port;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];
    int Port=SERV_POT;    //宏中自定义的端口号　后面还要改
    char client_IP[32]="127.0.0.1";//ＩＰ地址
    char username[MAX_USERNAME];
    char register_tmp[MAX_ACCOUNT];
    recv_file_t recv_file;
    recv_t Package;//登录请求时要发的包

    List_Init(head,node_friend_t);     //初始化好友链表 
    List_Init(group_head,node_group_t); //初始化群链表　其中包含全部群  

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET; //ipv4 tcp

    serv_port=Port;  //设置端口
    if(serv_port<0 || serv_port>65535)
    {
        perror("invaild port\n"); 
        exit(1);
    }else
    {
        serv_addr.sin_port=htons(serv_port);//转化为网络字节序
    }

    if(inet_aton(client_IP,&serv_addr.sin_addr)==0)//设置IP
    {
        perror("invaild server ip!\n");
        exit(1);
    }
    if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr==0) //unsigned long
    {
        perror("invaild param!\n");
        exit(1);
    }
    //ipv4 tcp/ip协议    　　　tcp流套接字
    conn_fd=socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd<0)
    {
        perror("socket\n");
        exit(1);
    }
    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0)
    {
        perror("目前用户数量太多！\n");//当用户数量太多时不连接　超过1024
        exit(1);
    }

    printf("连接请求已运行\n");
    char ch;
    int flag=0;
    int ans=0;      //巧妙的地方在于在登录时做了消息离线处理　登录一完成即显示离线消息　数据库实现
                    //所以后面主功能块可以开一个线程专门收消息　与前面并不冲突
    do{
        system("clear");
        printf("Register      [R]     Enter   [E]\n");
        printf("Back_password [B]     Quit    [Q]\n");
        scanf("%c",&ch);
        switch (ch)
        {
            case 'R':
            case 'r':
                register_client(conn_fd,register_tmp);
                break;
            case 'E':
            case 'e':
                if(!login_client(conn_fd,username))  //希望指针返回一个用户名
                {
                    printf("please login again.\n");
                    flag=1;
                }else ans=1;
                break;
            case 'B':
            case 'b':
                Retrieve_client(conn_fd);
                break;
            case 'Q':
            case 'q':
            default:
                break;
        }
        if(flag) break;
        if(ans)  break;
    }while(ch !='q' && ch!='Q');
    pthread_t pth1;
    if(!flag)
    {      //登录成功后显示的页面
    pthread_create(&pth1,NULL,method_client,NULL); //开一个线程专门收包
	char choice;
	 do { 
		system("clear");
		printf("\n\n====================================================================\n");
		printf("\n========您好，[%s]先生/女士,欢迎来到zhaolonga-happychat=========\n",username);
		printf("\n==================================================================\n");
		printf("     **************** zhaolonga-happychat ****************\n\n");
		printf("      [A]添加好友                [D]删除好友\n");
		printf("\n");
		printf("      [S]查看好友列表             [C]查看好友请求\n");
        //消息盒子中只显示好友请求　其他在与好友聊天时自动载入消息链表
		printf("                      \n");
		printf("      [N]查看群列表                [R]注册群\n");
		printf("                       \n");
		printf("      [J]加入群　　                [L]发送文件\n");
		printf("                       \n");
		printf("      [E]退出登录\n");
		printf("\n==================================================================\n");
		printf("\n请输入您要进行的操作");
		choice = getchar();
		switch (choice)
		 {
		case 'A':
		case 'a':
            Add_Friend(conn_fd);
			break;
         case 'D': 
		 case 'd':
		 	Del_Friend(conn_fd);
			 break; 
		case 'S':
		case 's':
            show_friend_list();
			break;
		case 'j': 
		case 'J': 
			system("clear");
            Add_group(conn_fd);
			break; 
		case 'R': 
		case 'r': 
            register_group_client(conn_fd);
			break; 
		case 'N': 
		case 'n': 
            show_group_list();
			break; 
/*         case 'c':
        case 'C':
            show_friend_request();
            break; */
		case 'L': 
		case 'l':
            system("clear");
            bzero(&recv_file,sizeof(recv_file_t));
            getchar();
            system("clear");
            printf("please enter an account who you want to send:\n");
            scanf("%s",recv_file.count);
            getchar();
            printf("please enter an absolute path to send:\n");
            scanf("%s",recv_file.path);
            pp_tmp=pthread_create(&pp_tmp,NULL,send_file,&recv_file);
            pthread_detach(pp_tmp);//设置为分离状态　防止阻塞主线程或者浪费资源(未被清理)
			break;
		}
	} while ('E' != choice && 'e' != choice); 
    } 
    List_Destroy(head, node_friend_t);//删除好友链表
    List_Destroy(group_head,node_group_t);
    List_Destroy(Message_BOX,node_messages_t);
    close(conn_fd);
    return 0;
}