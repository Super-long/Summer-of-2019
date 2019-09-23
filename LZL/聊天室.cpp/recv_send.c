#ifndef _MY_SEND_RECV_H
#define _MY_SEND_RECV_H

#include "Data.h"
#include <errno.h>

int sendn(int sockfd, void *buf, int len, int flags)
{
    int n = 0, sum = 0;
    const char *ptr = (const char *)buf;
    do
    {
        n = send(sockfd, (void *)ptr, len - sum, flags);
        if (n < 0)
        {
            if (errno == EINTR)
                n = 0;
            else
                return -1;
        }
        sum += n;
        ptr += n;
    } while (sum < len);
    // printf("sum %d\n",sum);
    return sum;
}

int recvn(int sockfd, void *buf, int len, int flags)
{
    // printf("对于刚刚的任务接收\n");
    int n = 0, sum = 0;
    char *ptr = (char *)buf;
    do
    {
        n = recv(sockfd, ptr, len - sum, flags);
        if (n < 0)
        {
            printf("sockfd %d \n",sockfd);
            return n;
        }
        if (n == 0)
        {
            printf(" recv 对方已断线\n");
            return 0 ;
        }
        sum += n;
        ptr += n;
        // printf("n = %d     ",n);
    } while (sum < len);
    // printf("size ----------------------%d\n",buf->size);
    // printf("服务器recv%s\n",buf->record);
    //printf("sum  %d \n",sum);
    return (sum);
}

#endif