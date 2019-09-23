#ifndef INCIDENT_H_
#define INCIDENT_H_
#include"incident.cpp"

int my_recv(int conn_fd,char *data_buf,int len);
int get_userinfo(char *buf,int len);
int input_userinfo(recv_t *temp);     //基础函数

//登录　注册　找回密码
int login_client(int conn_fd,char *username);
int register_client(int conn_fd,char *account);
int Retrieve_client(int conn_fd);

//添加　删除　查看好友
int Add_Friend(int conn_fd);

#endif INCIDENT_H_