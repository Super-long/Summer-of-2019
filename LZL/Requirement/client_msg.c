#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ipc.h>

#define BUF_SIZE 1024
#define PROJ_ID  32
#define PATHNAME "/tmp"
#define SERVER_MSG 1
#define CLIENT_MSG 2

int main(void)
{
    struct mymsgbuf{
        long msgtype;
        char ctrlstring[BUF_SIZE];
    }msgbuffer;
    int qid;
    int msglen;
    key_t msgkey;

    if((msgkey=ftok(PATHNAME,PROJ_ID))==-1)
    {
        perror("error in msgkey\n");
        exit(1);
    }
    if(qid=msgget(msgkey,IPC_CREAT|0660)==-1)
    {
        perror("error in msgget!\n");
        exit(1);
    }

    while(1)
    {
        printf("asdf\n");
        if(msgrcv(qid,&msgbuffer,BUF_SIZE,SERVER_MSG,0)==-1)//当没有信息是会阻塞
        {
            perror("error in msgrcv\n");
            exit(1);
        }
        printf("Server: %s\n",msgbuffer.ctrlstring);
        printf("Client:");
        fgets(msgbuffer.ctrlstring,BUF_SIZE,stdin);
        putchar('\n');
        if(!strncmp("quit",msgbuffer.ctrlstring,4))
        {
            msgctl(qid,IPC_RMID,NULL);
            exit(1);
        }
        msgbuffer.ctrlstring[strlen(msgbuffer.ctrlstring)-1]='\0';
        msgbuffer.msgtype=CLIENT_MSG;
        if(msgsnd(qid,&msgbuffer,strlen(msgbuffer.ctrlstring)+1,0)==-1)//切记传一个+1 否则会导致数据丢失
        {
            perror("error in msgsnd\n");
            exit(1);
        }
    }
    return 0;
}